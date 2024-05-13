#pragma once
#include <vector>
#include <random>
class IRangeOperator
{
public:

	virtual std::vector<double> operator()(std::vector<double> currentValue) = 0;
	virtual ~IRangeOperator();
	IRangeOperator(std::default_random_engine& generator);
	IRangeOperator();
protected:
	std::default_random_engine generator;
	
};

