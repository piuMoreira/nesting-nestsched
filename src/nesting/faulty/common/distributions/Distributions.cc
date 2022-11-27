//
// @author: Renan Silva
//

#include "nesting/faulty/common/distributions/Distributions.h"

namespace nesting {

    int Distributions::uniformDistribution(int upperBoundary) {
        std::random_device rd{}; // use to seed the rng
        std::mt19937 rng{rd()}; // rng
        std::uniform_int_distribution<int> distribution(1,upperBoundary);
//        float rand = (float) distribution(rng)/1000000;
//        dPar = (inet::simtime_t) rand;
        return distribution(rng);
    }

}
