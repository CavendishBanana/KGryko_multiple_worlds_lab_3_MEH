#include "IRangeOperator.h"


IRangeOperator::~IRangeOperator()
{
}

IRangeOperator::IRangeOperator(std::default_random_engine& generator) :generator(generator)
{
}

IRangeOperator::IRangeOperator() :generator(  std::default_random_engine()  )
{
}


