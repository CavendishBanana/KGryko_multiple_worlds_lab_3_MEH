#include "CauchyRangeOperator.h"

CauchyRangeOperator::CauchyRangeOperator(double gamma, double x0) :IRangeOperator(), gamma(gamma), x0(x0), distribution( std::cauchy_distribution<double>(x0, gamma) )
{
}

CauchyRangeOperator::CauchyRangeOperator(double gamma, double x0, std::default_random_engine& generator) :IRangeOperator(generator), gamma(gamma), x0(x0), distribution(std::cauchy_distribution<double>(x0, gamma))
{
}

std::vector<double> CauchyRangeOperator::operator()(std::vector<double> currentValue)
{
    //std::for_each(currentValue.begin(), currentValue.end(), [&cau = distribution, &gen = generator](double& current) {current += cau(gen); });
    for (unsigned int i = 0; i < currentValue.size(); i++)
    {
        currentValue[i] += distribution(generator);
    }
    return currentValue;
}

CauchyRangeOperator::~CauchyRangeOperator()
{
}
