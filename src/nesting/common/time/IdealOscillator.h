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

#ifndef __NESTINGNG_IDEALOSCILLATOR_H_
#define __NESTINGNG_IDEALOSCILLATOR_H_

#include <omnetpp.h>

#include <list>
#include <cstdint>
#include <iostream>

#include "nesting/common/time/IOscillator.h"

using namespace omnetpp;

namespace nesting {

class IdealOscillatorTick;

/** Implementation of an oscillator module without drift or jitter. */
class IdealOscillator : public cSimpleModule, public IOscillator
{
protected:
    /** Flag that is set to true mid tick event. */
    bool tickEventNow;

    /** Tick rate of the oscillator module in seconds. */
    double frequency;

    /** Number/Index of last tick. */
    uint64_t lastTick;

    /** Global simulation time when the last tick event was scheduled. */
    simtime_t timeOfLastTick;

    /** Event queue that contains the scheduled tick events. */
    std::list<IdealOscillatorTick*> scheduledEvents;

    /** Used as self message to notify the component of the next tick event */
    cMessage tickMessage;
public:
    IdealOscillator();

    virtual ~IdealOscillator();

    /** @copydoc IOscillator::subscribeTick() */
    virtual const IOscillatorTick* subscribeTick(IOscillatorListener* listener, uint64_t idleTicks, uint64_t kind = 0) override;

    /** @copydoc IOscillator::unsubscribeTick() */
    virtual void unsubscribeTick(IOscillatorListener* listener, const IOscillatorTick* tick) override;

    /** @copydoc IOscillator::unsubscribeTicks(IOscillatorListener*, uint64_t) */
    virtual void unsubscribeTicks(IOscillatorListener* listener, uint64_t kind) override;

    /** @copydoc IOscillator::unsubscribeTicks(IOscillatorListener*) */
    virtual void unsubscribeTicks(IOscillatorListener* listener) override;

    /** @copydoc IOscillator::isScheduled() */
    virtual bool isScheduled(IOscillatorListener* listener, const IOscillatorTick* tickEvent) const override;

    /** @copydoc IOscillator::getFrequency() */
    virtual double getFrequency() const override;

    /** @copydoc IOscillator::setFrequency() */
    virtual void setFrequency(double frequency) override;

    /** @copydoc IOscillator::updateAndGetCurrentTick() */
    virtual uint64_t updateAndGetCurrentTick() override;
protected:
    virtual void initialize() override;

    virtual void finish() override;

    virtual void handleMessage(cMessage *msg) override;

    virtual simtime_t getTickInterval() const;
    /**
     * Schedules a self-message so the component is notified about the next
     * event in the event-queue. Reschedules an already existent self-message
     * if required.
     *
     * This method should be called after every update to the event queue.
     */
    virtual void scheduleNextTick();
};

class IdealOscillatorTick : public IOscillatorTick {
protected:
    IOscillatorListener* listener;

    uint64_t tick;

    uint64_t kind;
public:
    IdealOscillatorTick();

    IdealOscillatorTick(const IOscillatorTick& tickEvent);

    virtual ~IdealOscillatorTick();

    virtual IOscillatorListener* getListener() const;

    virtual void setListener(IOscillatorListener* listener);

    virtual uint64_t getTick() const override;

    virtual void setTick(uint64_t tick);

    virtual uint64_t getKind() const override;

    virtual void setKind(uint64_t kind);

    bool operator<(const IdealOscillatorTick& tickEvent) const;

    bool operator==(const IdealOscillatorTick& tickEvent) const;

    bool operator!=(const IdealOscillatorTick& tickEvent) const;

    friend std::ostream& operator<<(std::ostream& stream, const IdealOscillatorTick* tickEvent);
};

std::ostream& operator<<(std::ostream& stream, const IdealOscillatorTick* tickEvent);

} //namespace nesting

#endif
