#pragma once
#include "IRangeOperator.h"
class NormalRangeOperator :
    public IRangeOperator
{
private:
    double mi, sigma;
    std::normal_distribution<double> distribution;

public:
    NormalRangeOperator(double mi, double sigma, std::default_random_engine& generator);
    NormalRangeOperator(double mi, double sigma);
    std::vector<double> operator()(std::vector<double> currentValue) override;
    virtual ~NormalRangeOperator();

};

