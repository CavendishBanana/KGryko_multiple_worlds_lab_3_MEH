#include "UniformRangeOperator.h"

UniformRangeOperator::UniformRangeOperator(double leftEnd, double rightEnd) :IRangeOperator(), leftEnd(leftEnd), rightEnd(rightEnd), distribution(leftEnd, rightEnd)
{
}

UniformRangeOperator::UniformRangeOperator(double leftEnd, double rightEnd, std::default_random_engine& generator) :IRangeOperator(generator), leftEnd(leftEnd), rightEnd(rightEnd), distribution(leftEnd, rightEnd)
{
}

std::vector<double> UniformRangeOperator::operator()(std::vector<double> currentValue)
{
    std::for_each(currentValue.begin(), currentValue.end(), [&cau = distribution, &gen = generator](double& current) {current += cau(gen); });
    return currentValue;
}
