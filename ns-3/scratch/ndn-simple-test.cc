// ndn-simple.cpp

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/ndnSIM/helper/ndn-link-control-helper.hpp"

namespace ns3 {

int
main(int argc, char* argv[])
{
  // setting default parameters for PointToPoint links and channels
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("10Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
  Config::SetDefault("ns3::DropTailQueue::MaxPackets", StringValue("20"));

  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
  CommandLine cmd;
  cmd.Parse(argc, argv);

  // Creating nodes
  NodeContainer nodes;
  nodes.Create(6);

  // Connecting nodes using two links
  PointToPointHelper p2p;
  p2p.Install(nodes.Get(0), nodes.Get(1));
  p2p.Install(nodes.Get(1), nodes.Get(2));
  p2p.Install(nodes.Get(1), nodes.Get(3));
  p2p.Install(nodes.Get(2), nodes.Get(4));
  p2p.Install(nodes.Get(3), nodes.Get(4));
  p2p.Install(nodes.Get(4), nodes.Get(5));

  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.SetDefaultRoutes(true);
  ndnHelper.InstallAll();

  // Choosing forwarding strategy
  ndn::StrategyChoiceHelper::InstallAll("/prefix", "/localhost/nfd/strategy/broadcast");

  // Installing applications

  // Consumer
  ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
  // Consumer will request /prefix/0, /prefix/1, ...
  consumerHelper.SetPrefix("/prefix");
  consumerHelper.SetAttribute("Frequency", StringValue("10")); // 10 interests a second
  consumerHelper.Install(nodes.Get(0));                        // first node

  // Producer
  ndn::AppHelper producerHelper("ns3::ndn::Producer");
  // Producer will reply to all requests starting with /prefix
  producerHelper.SetPrefix("/prefix");
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));

  // ndn::AppHelper producerHelper("Hijacker");
  producerHelper.Install(nodes.Get(5)); // last node

  //   // Calculate and install FIBs
  // ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  // ndnGlobalRoutingHelper.InstallAll();
  // ndnGlobalRoutingHelper.AddOrigins("/prefix", nodes.Get(5));
  // ndn::GlobalRoutingHelper::CalculateRoutes();

  Simulator::Schedule(Seconds(10.0), ndn::LinkControlHelper::FailLink, nodes.Get(2), nodes.Get(4));
  Simulator::Schedule(Seconds(20.0), ndn::LinkControlHelper::UpLink, nodes.Get(2), nodes.Get(4));
  Simulator::Schedule(Seconds(30.0), ndn::LinkControlHelper::FailLink, nodes.Get(4), nodes.Get(2));
  Simulator::Schedule(Seconds(40.0), ndn::LinkControlHelper::UpLink, nodes.Get(4), nodes.Get(2));

  Simulator::Schedule(Seconds(45.0), ndn::LinkControlHelper::FailLink, nodes.Get(2), nodes.Get(3));
  Simulator::Schedule(Seconds(50.0), ndn::LinkControlHelper::UpLink, nodes.Get(2), nodes.Get(3));
  Simulator::Schedule(Seconds(55.0), ndn::LinkControlHelper::FailLink, nodes.Get(3), nodes.Get(2));
  Simulator::Schedule(Seconds(60.0), ndn::LinkControlHelper::UpLink, nodes.Get(3), nodes.Get(2));


  Simulator::Stop(Seconds(100.0));

  Simulator::Run();
  Simulator::Destroy();

  return 0;
}

} // namespace ns3

int
main(int argc, char* argv[])
{
  return ns3::main(argc, argv);
}