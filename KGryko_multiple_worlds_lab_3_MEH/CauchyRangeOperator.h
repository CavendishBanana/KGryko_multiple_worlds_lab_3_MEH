#pragma once
#include "IRangeOperator.h"
#include <vector>
#include <random>
#include <algorithm>
class CauchyRangeOperator :
    public IRangeOperator
{
private:
    double gamma, x0;
    std::cauchy_distribution<double> distribution;

public:
    
    CauchyRangeOperator(double gamma, double x0 );
    CauchyRangeOperator(double gamma, double x0, std::default_random_engine& generator);
    std::vector<double> operator()(std::vector<double> currentValue) override;
    virtual ~CauchyRangeOperator();
};

