#pragma once
#include "IRangeOperator.h"
#include <random>
class UniformRangeOperator :
    public IRangeOperator
{
private:
    std::uniform_real_distribution<double> distribution;
    double leftEnd, rightEnd;

public:
    UniformRangeOperator(double leftEnd, double rightEnd);
    UniformRangeOperator(double leftEnd, double rightEnd, std::default_random_engine& generator);
    std::vector<double> operator()(std::vector<double> currentValue) override;

};

