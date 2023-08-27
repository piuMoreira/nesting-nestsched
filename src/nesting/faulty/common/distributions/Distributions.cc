//
// @author: Renan Silva
//

#include "nesting/faulty/common/distributions/Distributions.h"
#include <iostream>

namespace nesting {

    int Distributions::distributionRun(int distType, int upperBoundary) {
        int value=0;
        switch (distType) {
            case UNIFORM_DISTRIBUTION:
                value = uniformDistribution(upperBoundary);
                break;
            case POISSON_DISTRIBUTION:
                value = poissonDistribution(upperBoundary);
                break;
            case LOGNORMAL_DISTRIBUTION:
                value = lognormalDistribution(upperBoundary);
                break;
        }
        return value;
    }

    int Distributions::uniformDistribution(int upperBoundary) {
        std::random_device rd{}; // use to seed the rng
        std::mt19937 rng{rd()}; // rng
        std::uniform_int_distribution<int> distribution(1,upperBoundary);
        return distribution(rng);
    }

    int Distributions::poissonDistribution(int upperBoundary) {
        std::random_device rd{}; // use to seed the rng
        std::mt19937 rng{rd()}; // rng
        std::poisson_distribution<int>  distribution(upperBoundary);
        return distribution(rng);
    }

    int Distributions::lognormalDistribution(int upperBoundary) {
        std::random_device rd{}; // use to seed the rng
        std::mt19937 rng{rd()}; // rng
        std::lognormal_distribution<float> distribution(0.0,100.0);
        return std::round(distribution(rng));
    }


}
