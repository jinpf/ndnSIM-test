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

#include "ndn-producer-push.hpp"
#include "ns3/log.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/double.h"

#include "model/ndn-app-face.hpp"
#include "model/ndn-ns3.hpp"
#include "model/ndn-l3-protocol.hpp"
#include "helper/ndn-fib-helper.hpp"

#include <iostream>

#include <memory>

NS_LOG_COMPONENT_DEFINE("ndn.ProducerP");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED(ProducerP);

TypeId
ProducerP::GetTypeId(void)
{
  static TypeId tid =
    TypeId("ns3::ndn::ProducerP")
      .SetGroupName("Ndn")
      .SetParent<App>()
      .AddConstructor<ProducerP>()
      .AddAttribute("Prefix", "Prefix, for which producer has the data", StringValue("/"),
                    MakeNameAccessor(&ProducerP::m_prefix), MakeNameChecker())
      .AddAttribute(
         "Postfix",
         "Postfix that is added to the output data (e.g., for adding producer-uniqueness)",
         StringValue("/"), MakeNameAccessor(&ProducerP::m_postfix), MakeNameChecker())
      .AddAttribute("PayloadSize", "Virtual payload size for Content packets", UintegerValue(1024),
                    MakeUintegerAccessor(&ProducerP::m_virtualPayloadSize),
                    MakeUintegerChecker<uint32_t>())
      .AddAttribute("Freshness", "Freshness of data packets, if 0, then unlimited freshness",
                    TimeValue(Seconds(0)), MakeTimeAccessor(&ProducerP::m_freshness),
                    MakeTimeChecker())
      .AddAttribute(
         "Signature",
         "Fake signature, 0 valid signature (default), other values application-specific",
         UintegerValue(0), MakeUintegerAccessor(&ProducerP::m_signature),
         MakeUintegerChecker<uint32_t>())
      .AddAttribute("KeyLocator",
                    "Name to be used for key locator.  If root, then key locator is not used",
                    NameValue(), MakeNameAccessor(&ProducerP::m_keyLocator), MakeNameChecker())

      .AddAttribute("Frequency", "Frequency of data packet generate", StringValue("1.0"),
                    MakeDoubleAccessor(&ProducerP::m_frequency), MakeDoubleChecker<double>())

      .AddAttribute("Randomize",
                    "Type of send time randomization: none (default), uniform, exponential",
                    StringValue("none"),
                    MakeStringAccessor(&ProducerP::SetRandomize, &ProducerP::GetRandomize),
                    MakeStringChecker())
      .AddTraceSource("PacketRecord",
                      "Record data send and receive in file",
                      MakeTraceSourceAccessor(&ProducerP::m_PacketRecord));

      // above code comes from ndn-producer and ndn-consumer-cbr

  return tid;
}

ProducerP::ProducerP()
  : m_seq(0)
  , m_frequency(1.0)
  , m_random(0)
  , m_subscribe(false)
{
  NS_LOG_FUNCTION_NOARGS();
}

ProducerP::~ProducerP()
{
  if (m_random)
    delete m_random;
}

// inherited from Application base class.
void
ProducerP::StartApplication()
{
  NS_LOG_FUNCTION_NOARGS();
  App::StartApplication();

  FibHelper::AddRoute(GetNode(), m_prefix, m_face, 0);

  ScheduleNextData();
}

void
ProducerP::StopApplication()
{
  NS_LOG_FUNCTION_NOARGS();

  App::StopApplication();
}

void
ProducerP::SendData(const Name &dataName)
{
  auto data = make_shared<Data>();
  data->setName(dataName);
  data->setFreshnessPeriod(::ndn::time::milliseconds(m_freshness.GetMilliSeconds()));

  data->setContent(make_shared< ::ndn::Buffer>(m_virtualPayloadSize));

  Signature signature;
  SignatureInfo signatureInfo(static_cast< ::ndn::tlv::SignatureTypeValue>(255));

  if (m_keyLocator.size() > 0) {
    signatureInfo.setKeyLocator(m_keyLocator);
  }

  signature.setInfo(signatureInfo);
  signature.setValue(Block(&m_signature, sizeof(m_signature)));

  data->setSignature(signature);

  NS_LOG_INFO("node(" << GetNode()->GetId() << ") respodning with Data: " << data->getName());

  // to create real wire encoding
  data->wireEncode();

  m_transmittedDatas(data, this, m_face);
  m_face->onReceiveData(*data);

  m_PacketRecord(this, "P_Data", dataName.toUri(), dataName.at(-1).toSequenceNumber(), 0);

  // cout name in string
  // std::cout << dataName.toUri() << std::endl;
  // std::cout << dataName.at(-2).toUri() << std::endl;
  // cout sequecenumber in int
  std::cout << " send data:" << dataName.at(-1).toSequenceNumber() << std::endl;
}

void
ProducerP::OnInterest(shared_ptr<const Interest> interest)
{
  App::OnInterest(interest); // tracing inside

  NS_LOG_FUNCTION(this << interest);

  if (!m_active)
    return;

  Name dataName(interest->getName());
  // dataName.append(m_postfix);
  // dataName.appendVersion();

  if (m_prefix.isPrefixOf(dataName)) {
    m_subscribe = true;
    // record and log in file
    m_PacketRecord(this, "P_Interest", dataName.toUri(), -1, -1);
    std::cout << "[producer]receive comsumer subscribe: " << std::endl ;
  }
 
}

void
ProducerP::ScheduleNextData()
{
  if ( !m_seq) {
    m_generateEvent = Simulator :: Schedule(Seconds(0.0), &ProducerP::GenerateData, this);
  }
  else if ( !m_generateEvent.IsRunning()) {
    m_generateEvent = Simulator :: Schedule((m_random == 0) ? Seconds(1.0 / m_frequency)
                                                                                                       : Seconds(m_random->GetValue()),
                                                                          &ProducerP::GenerateData, this);
  }
}

// Attention! not really generate data, just add seq number pretend to generate data 
void
ProducerP::GenerateData()
{
  if (m_seq != m_seqmax) {
    // generate data and plus seq number
    m_seq++;
    // record and log in file
    m_PacketRecord(this, "P_GData", m_prefix.toUri(), m_seq, 0);
    std::cout << "[producer]generate data:" << m_seq << std::endl;
    // schedule to generate next data

    //if some one subscribe, then push data
    if (m_subscribe) {
      Name nameWithSequence(m_prefix);
      nameWithSequence.appendSequenceNumber(m_seq);
      SendData(nameWithSequence);
    }
    
    ScheduleNextData();
  }
}

void
ProducerP::SetRandomize(const std::string &value)
{
  if (m_random)
    delete m_random;

  if (value == "uniform") {
    m_random = new UniformVariable(0.0, 2 * 1.0 / m_frequency);
  }
  else if (value == "exponential") {
    m_random = new ExponentialVariable(1.0 / m_frequency, 50 * 1.0 / m_frequency);
  }
  else
    m_random = 0;
  m_randomType = value;
}

std::string
ProducerP::GetRandomize() const
{
  return m_randomType;
}

} // namespace ndn
} // namespace ns3
