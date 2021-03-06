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

#ifndef NDN_PRODUCER_P_H
#define NDN_PRODUCER_P_H

#include "ns3/ndnSIM/model/ndn-common.hpp"

#include "ndn-app.hpp"
#include "ns3/ndnSIM/model/ndn-common.hpp"

#include "ns3/nstime.h"
#include "ns3/ptr.h"

#include "ns3/random-variable.h"

namespace ns3 {
namespace ndn {

/**
 * @ingroup ndn-apps
 * @brief A simple Interest-sink applia simple Interest-sink application
 *
 * A simple Interest-sink applia simple Interest-sink application,
 * which replying every incoming Interest with Data packet with a specified
 * size and name same as in Interest.cation, which replying every incoming Interest
 * with Data packet with a specified size and name same as in Interest.
 * 
 * The difference between ProducerP and Producer is that, Producer respond data
 * immediately after receiving Interest packet. But ProducerP generate data at a 
 * random time, and directly push it.
 */
class ProducerP : public App {
public:
  static TypeId
  GetTypeId(void);

  ProducerP();
  virtual ~ProducerP();

  // senddata with name
  void
  SendData(const Name &dataname);

  // inherited from NdnApp
  virtual void
  OnInterest(shared_ptr<const Interest> interest);

protected:

  uint32_t m_seq;  // current produced sequence number
  double m_frequency;  // frequency of data packet gererating in 1 second
  RandomVariable *m_random;  // random
  std::string m_randomType; //  random type: uniform or exponential
  EventId m_generateEvent; // EventId of generate data event

  // inherited from Application base class.
  virtual void
  StartApplication(); // Called at time specified by Start

  virtual void
  StopApplication(); // Called at time specified by Stop

 // schedule next generate
  virtual void
  ScheduleNextData();

  // generate data
  virtual void
  GenerateData();

  // set type of frequency randomization, 'none' 'uniform' 'exponential'
  void
  SetRandomize(const std::string &value);
  
  // get randomization type
  std::string
  GetRandomize() const;

  // added to record information by using tracers
  TracedCallback<Ptr<App> , std::string , std::string , int32_t, int32_t >
    m_PacketRecord;

private:
  Name m_prefix;
  Name m_postfix;
  uint32_t m_virtualPayloadSize;
  Time m_freshness;
  bool m_subscribe;

  uint32_t m_signature;
  Name m_keyLocator;
};

} // namespace ndn
} // namespace ns3

#endif // NDN_PRODUCER_P_H
