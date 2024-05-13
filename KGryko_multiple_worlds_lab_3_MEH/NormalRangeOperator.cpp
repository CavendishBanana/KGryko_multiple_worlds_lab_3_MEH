#include "NormalRangeOperator.h"

NormalRangeOperator::NormalRangeOperator(double mi, double sigma) : IRangeOperator(), mi(mi), sigma(sigma)
{
}

NormalRangeOperator::NormalRangeOperator(double mi, double sigma, std::default_random_engine& generator) : IRangeOperator(generator), mi(mi), sigma(sigma)
{
}

std::vector<double> NormalRangeOperator::operator()(std::vector<double> currentValue)
{
    std::for_each(currentValue.begin(), currentValue.end(), [&cau = distribution, &gen = generator](double& current) {current += cau(gen); });
    return currentValue;
}

NormalRangeOperator::~NormalRangeOperator()
{
}
