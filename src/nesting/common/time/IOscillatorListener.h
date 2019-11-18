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

#ifndef NESTING_COMMON_TIME_IOSCILLATORLISTENER_H_
#define NESTING_COMMON_TIME_IOSCILLATORLISTENER_H_

#include <cstdint>
#include <memory>

#include "nesting/common/time/IOscillator.h"

namespace nesting {

class IOscillator;
class IOscillatorTick;

class IOscillatorListener {
public:
    virtual ~IOscillatorListener() {};

    virtual void onOscillatorTick(IOscillator& oscillator, const IOscillatorTick& tick) {};

    virtual void onOscillatorFrequencyChange(IOscillator& oscillator, double oldFrequency, double newFrequency) {};
};

} // namespace nesting

#endif /* NESTING_COMMON_TIME_IOSCILLATORLISTENER_H_ */
