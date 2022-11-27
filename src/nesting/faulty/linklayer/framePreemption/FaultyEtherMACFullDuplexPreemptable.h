//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#ifndef __FAULTY_ETHERMACFULLDUPLEXPREEMPTABLE_H
#define __FAULTY_ETHERMACFULLDUPLEXPREEMPTABLE_H

#include "inet/common/INETDefs.h"
#include "inet/common/queue/IPassiveQueue.h"
#include "inet/linklayer/ethernet/EtherMacFullDuplex.h"
#include "inet/linklayer/ethernet/EtherEncap.h"
#include "inet/linklayer/ethernet/EtherPhyFrame_m.h"

#include "nesting/ieee8021q/queue/TransmissionSelection.h"
#include "nesting/ieee8021q/Ieee8021q.h"

using namespace inet;

namespace nesting {

class TransmissionSelection;
/**
 * A simplified version of EtherMAC. Since modern Ethernets typically
 * operate over duplex links where's no contention, the original CSMA/CD
 * algorithm is no longer needed. This simplified implementation doesn't
 * contain CSMA/CD, frames are just simply queued up and sent out one by one.
 */
class FaultyEtherMACFullDuplexPreemptable: public EtherMacFullDuplex,
        public IPassiveQueueListener {
private:
    TransmissionSelection* transmissionSelectionModule;

    cMessage *recheckForQueuedExpressFrameMsg = nullptr;
    cMessage *preemptCurrentFrameMsg = nullptr;

    //cMessage recheckForQueuedExpressFrameMsg = cMessage("RecheckForQueuedExpressFrame");
    //cMessage preemptCurrentFrameMsg = cMessage("preemptCurrentFrame");
//    cMessage *recheckForQueuedExpressFrameMsg = nullptr;
//    cMessage *preemptCurrentFrameMsg = nullptr;

    bool transmittingExpressFrame = false;
    Packet* currentExpressFrame = nullptr;

    bool onHold = false;
    bool transmittingPreemptableFrame = false;
    Packet* currentPreemptableFrame = nullptr;
    simtime_t preemptableTransmissionStart;

    simtime_t pFrameArrivalTime;
    simtime_t eFrameArrivalTime;

    unsigned int preemptedBytesReceived;
    unsigned int preemptedBytesSent;
    EthernetSignal* receivedPreemptedFrame = nullptr;

    virtual int calculatePreemptedPayloadBytesSent(simtime_t timeToCheck, bool sentCRC);
    virtual bool isPreemptionNowPossible();
    virtual simtime_t isPreemptionLaterPossible();
    virtual simtime_t calculateTransmissionDuration(int bytes);
    virtual void preemptCurrentFrame();
protected:

    virtual void initialize(int stage) override;
    virtual void handleMessageWhenUp(cMessage *msg) override;
    virtual void handleSelfMessage(cMessage *msg) override;
    virtual void handleEndTxPeriod() override;
    virtual void handleEndIFGPeriod() override;
    virtual void getNextFrameFromQueue() override;
    virtual bool checkForAndRequestExpressFrame();
    virtual void handleUpperPacket(Packet *frame) override;
    virtual void startFrameTransmission() override;
    virtual void refreshDisplay() const override;
    virtual bool isExpressFrame(Packet* packet) const;
public:
    virtual void packetEnqueued(IPassiveQueue *queue);
    virtual void hold(simtime_t delay);
    virtual void release();
    virtual simtime_t getHoldAdvance();
    virtual bool isOnHold();
    ~FaultyEtherMACFullDuplexPreemptable();
    virtual bool isFramePreemptionEnabled();
};

}
// namespace inet

#endif // ifndef __FAULTY_ETHERMACFULLDUPLEXPREEMPTABLE_H

