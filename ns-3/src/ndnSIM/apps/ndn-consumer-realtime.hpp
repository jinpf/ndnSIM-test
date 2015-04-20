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

#ifndef NDN_CONSUMERR_H
#define NDN_CONSUMERR_H

#include "ns3/ndnSIM/model/ndn-common.hpp"

#include "ndn-app.hpp"

#include "ns3/random-variable.h"
#include "ns3/nstime.h"

#include "ns3/ndnSIM/model/ndn-common.hpp"
#include "ns3/ndnSIM/utils/ndn-rtt-estimator.hpp"
#include "ns3/ndnSIM/utils/ndn-fw-hop-count-tag.hpp"

namespace ns3 {
namespace ndn {

/**
 * @ingroup ndn-apps
 * \brief NDN application for sending out Interest packets
 */
class ConsumerR : public App {
public:
  static TypeId
  GetTypeId();

  /**
   * \brief Default constructor
   * Sets up randomizer function and packet sequence number
   */
  ConsumerR();
  virtual ~ConsumerR(){};

  // From App
  void
  OnData(shared_ptr<const Data> contentObject);

  /**
   * @brief Actually send packet
   */
  void
  SendPacket(uint32_t sequenceNumber);

  void
  RetxPacket();

protected:
  // from App
  virtual void
  StartApplication();

  virtual void
  StopApplication();

  // added to record information by using tracers
  TracedCallback<Ptr<App> , std::string , std::string , int32_t, int32_t >
    m_PacketRecord;

protected:
  UniformVariable m_rand; ///< @brief nonce generator

  uint32_t m_seq;      ///< @brief currently requested sequence number
  uint32_t r_seq;     /// retx sequence number
  EventId m_sendEvent; ///< @brief EventId of pending "send packet" event
  Time m_retxTimer;    ///< @brief Currently estimated retransmission timer
  EventId m_retxEvent; ///< @brief Event to check whether or not retransmission should be performed

  Ptr<RttEstimator> m_rtt; ///< @brief RTT estimator

  Name m_interestName;     ///< \brief NDN Name of the Interest (use Name)
  Time m_interestLifeTime; ///< \brief LifeTime for interest packet

};
} // namespace ndn
} // namespace ns3

#endif