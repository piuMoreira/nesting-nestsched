//
// @author: Renan Silva
//

#ifndef _FAULTY_DISTRIBUTIONS_H
#define _FAULTY_DISTRIBUTIONS_H

#include <random>
#include <iostream>

namespace nesting {

enum distributionsEnum {
    UNIFORM_DISTRIBUTION = 1,
    POISSON_DISTRIBUTION = 2,
    LOGNORMAL_DISTRIBUTION = 3
};

class Distributions {
    public:
        int distributionRun(int distType, int upperBoundary);
        int uniformDistribution(int upperBoundary);
        int poissonDistribution(int upperBoundary);
        int lognormalDistribution(int upperBoundary);
};

} // namespace nesting

#endif // ifndef _FAULTY_DISTRIBUTIONS_H
