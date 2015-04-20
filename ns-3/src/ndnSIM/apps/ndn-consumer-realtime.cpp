/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "ndn-consumer-realtime.hpp"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/string.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/integer.h"
#include "ns3/double.h"

#include "utils/ndn-ns3-packet-tag.hpp"
#include "model/ndn-app-face.hpp"
#include "utils/ndn-rtt-mean-deviation.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/ref.hpp>

NS_LOG_COMPONENT_DEFINE("ndn.ConsumerR");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED(ConsumerR);

TypeId
ConsumerR::GetTypeId(void)
{
  static TypeId tid =
    TypeId("ns3::ndn::ConsumerR")
      .SetGroupName("Ndn")
      .SetParent<App>()
      .AddConstructor<ConsumerR>()

      .AddAttribute("StartSeq", "Initial sequence number", IntegerValue(1),
                    MakeIntegerAccessor(&ConsumerR::m_seq), MakeIntegerChecker<int32_t>())

      .AddAttribute("Prefix", "Name of the Interest", StringValue("/"),
                    MakeNameAccessor(&ConsumerR::m_interestName), MakeNameChecker())
      .AddAttribute("LifeTime", "LifeTime for interest packet", StringValue("2s"),
                    MakeTimeAccessor(&ConsumerR::m_interestLifeTime), MakeTimeChecker())

      .AddAttribute("RetxTimer",
                    "Timeout defining how frequent retransmission timeouts should be checked",
                    StringValue("50ms"),MakeTimeAccessor(&ConsumerR::m_retxTimer),
                    MakeTimeChecker())

      .AddTraceSource("PacketRecord",
                      "Record data send and receive in file",
                      MakeTraceSourceAccessor(&ConsumerR::m_PacketRecord));

  return tid;
}

ConsumerR::ConsumerR()
  : m_rand(0, std::numeric_limits<uint32_t>::max())
  , m_seq(1)
{
  NS_LOG_FUNCTION_NOARGS();

  m_rtt = CreateObject<RttMeanDeviation>();
}

// Application Methods
void
ConsumerR::StartApplication() // Called at time specified by Start
{
  NS_LOG_FUNCTION_NOARGS();

  // do base stuff
  App::StartApplication();

  SendPacket(m_seq);
}

void
ConsumerR::StopApplication() // Called at time specified by Stop
{
  NS_LOG_FUNCTION_NOARGS();

  // cancel periodic packet generation
  Simulator::Cancel(m_sendEvent);

  // cleanup base stuff
  App::StopApplication();
}

void
ConsumerR::SendPacket(uint32_t sequenceNumber)
{
  if (!m_active)
    return;

  NS_LOG_FUNCTION_NOARGS();

  //
  shared_ptr<Name> nameWithSequence = make_shared<Name>(m_interestName);
  // nameWithSequence->append("pull");
  nameWithSequence->appendSequenceNumber(sequenceNumber);
  //

  // shared_ptr<Interest> interest = make_shared<Interest> ();
  shared_ptr<Interest> interest = make_shared<Interest>();
  interest->setNonce(m_rand.GetValue());
  interest->setName(*nameWithSequence);
  time::milliseconds interestLifeTime(m_interestLifeTime.GetMilliSeconds());
  interest->setInterestLifetime(interestLifeTime);

  // NS_LOG_INFO ("Requesting Interest: \n" << *interest);
  NS_LOG_INFO("> Interest for " << sequenceNumber);

  m_transmittedInterests(interest, this, m_face);
  m_face->onReceiveInterest(*interest);

  // log record in file
  m_PacketRecord(this, "C_Interest", nameWithSequence->toUri(), sequenceNumber, 0);
  
  r_seq = sequenceNumber;
  m_retxEvent = Simulator::Schedule(m_retxTimer, &ConsumerR::RetxPacket, this);
}

void
ConsumerR::RetxPacket()
{
  SendPacket(r_seq);
}

///////////////////////////////////////////////////
//          Process incoming packets             //
///////////////////////////////////////////////////

void
ConsumerR::OnData(shared_ptr<const Data> data)
{
  if (!m_active)
    return;

  App::OnData(data); // tracing inside

  NS_LOG_FUNCTION(this << data);

  if (m_retxEvent.IsRunning()) {
    // m_retxEvent.Cancel (); // cancel any scheduled cleanup events
    Simulator::Remove(m_retxEvent); // slower, but better for memory
  }

  // NS_LOG_INFO ("Received content object: " << boost::cref(*data));

  // This could be a problem......
  uint32_t seq = data->getName().at(-1).toSequenceNumber();
  NS_LOG_INFO("< DATA for " << seq);

  int hopCount = -1;
  auto ns3PacketTag = data->getTag<Ns3PacketTag>();
  if (ns3PacketTag != nullptr) {
    FwHopCountTag hopCountTag;
    if (ns3PacketTag->getPacket()->PeekPacketTag(hopCountTag)) {
      hopCount = hopCountTag.Get();
      NS_LOG_DEBUG("Hop count: " << hopCount);
    }
  }

  m_rtt->AckSeq(SequenceNumber32(seq));
  // log record in file
  m_PacketRecord(this, "C_Data", data->getName().toUri(), seq, hopCount);

  m_seq++;
  SendPacket(m_seq);
}

} // namespace ndn
} // namespace ns3
