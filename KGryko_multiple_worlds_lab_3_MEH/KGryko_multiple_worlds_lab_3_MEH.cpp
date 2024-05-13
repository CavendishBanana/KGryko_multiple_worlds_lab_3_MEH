
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <random>
#include <limits>
#include "IRangeOperator.h"
#include "UniformRangeOperator.h"
#include "CauchyRangeOperator.h"
#include "NormalRangeOperator.h"
#include <string>
#define twoPi 3.14159265358979323846
#define ePlus20 22.71828182845904523536

unsigned int globalRunsCountInAlgorithm;
using namespace std;

double ackleyFunction(const std::vector<double> &x)
{
    double oneByDimensionality = 1.0 / static_cast<double>(x.size());
    double sum1 = 0.0;
    double sum2 = 0.0;
    auto actionSum = [&sum1, &sum2](const double& x) { sum1 += (x * x); sum2 += cos(twoPi * x); };
    std::for_each(x.begin(), x.end(), actionSum);
    return ePlus20 - 20.0 * exp(-0.2 * sqrt(oneByDimensionality) * sum1) - exp(oneByDimensionality * sum2);
}

vector<double> initWithUniformRandomValues(unsigned int length, std::default_random_engine& generator)
{
    constexpr double limiter = 1.0e-100;
    constexpr double low =  limiter*numeric_limits<double>::lowest();
    constexpr double high = limiter* numeric_limits<double>::max();

    //std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(low, high);
    vector<double> vec(length, 0.0);
    for (unsigned int i = 0; i < length; i++)
    {
        vec[i] = distribution(generator);
    }
    return vec;
}

bool continueCondition1(double targetFunction, unsigned int runsCount)
{
    
    return runsCount < globalRunsCountInAlgorithm;
}

bool continueCondition2(double targetFunction, unsigned int runsCount)
{

    return targetFunction > 0.000001 && runsCount < 4 * globalRunsCountInAlgorithm;
}

bool ackleyFunOptimizationType(double left, double right)
{
    return left < right;
}

vector<double> multipleWorlds2ndVariant( default_random_engine& generator ,unsigned int dimensionality, unsigned int solutionsCount, bool (*continueCondition)(double, unsigned int), bool (*leftIsBetterThanRight)(double, double), double (*targetFunction)(const vector<double>&), IRangeOperator& closeRangeOperator, IRangeOperator& longRangeOperator, double closeOrLongRangeThresholdParam)
{
    //init obiects and find best starting solution
    
    uniform_real_distribution<double> closeOrLongRangeRandomGenerator = uniform_real_distribution<double>(0.0,1.0);
    vector<vector<double> > baseSolutions(solutionsCount, vector<double>(1, 0.0));
    vector<double> bestSolution = initWithUniformRandomValues(dimensionality, generator);
    double bestSolutionTargetFunValue = targetFunction(bestSolution);
    for (unsigned int i = 0; i < solutionsCount; i++)
    {
        baseSolutions[i] = initWithUniformRandomValues(dimensionality, generator);
        if (leftIsBetterThanRight(targetFunction(baseSolutions[i]), bestSolutionTargetFunValue))
        {
            bestSolution = baseSolutions[i];
            bestSolutionTargetFunValue = targetFunction(baseSolutions[i]);
        }
    }
    
    unsigned int runsCount = 0;

    //run the algorithm
    while (continueCondition(bestSolutionTargetFunValue, runsCount))
    {
        for (int i = 0; i < baseSolutions.size(); i++)
        {
            double randValue = closeOrLongRangeRandomGenerator(generator);
            vector<double> newSolution = vector<double>();
            if (randValue < closeOrLongRangeThresholdParam)
            {
                newSolution = closeRangeOperator(baseSolutions[i]);
            }
            else
            {
                newSolution = longRangeOperator(baseSolutions[i]);
            }
            double newSolutionTargetFun = targetFunction(newSolution);
            double baseSolutionTargetFun = targetFunction(baseSolutions[i]);
            if (leftIsBetterThanRight( newSolutionTargetFun, baseSolutionTargetFun))
            {
                baseSolutions[i] = newSolution;
            }
            if (leftIsBetterThanRight(newSolutionTargetFun, bestSolutionTargetFunValue))
            {
                bestSolutionTargetFunValue = newSolutionTargetFun;
                bestSolution = newSolution;
            }


        }
        runsCount++;
    }


    return bestSolution;
}

void printVector(const vector<double>& vec, unsigned int decimalPlacesCount = 4)
{
    //unsigned int decimalPlacesCount = 4;
    double powerOf10 = 1.0;
    for (int i = 0; i < decimalPlacesCount; i++)
    {
        powerOf10 *= 10.0;
    }
    cout << "(";
    for (int i = 0; i < vec.size(); i++)
    {
        double rounded = static_cast<double>(static_cast<int>(vec[i] * powerOf10)) / powerOf10;
        if (i != vec.size() - 1)
        {
            cout << setprecision(decimalPlacesCount) << rounded << " ";
        }
    }
    cout << ")";

}



int main()
{
    //test();

    globalRunsCountInAlgorithm = 10000;
    unsigned int dimensionality = 4;
    default_random_engine baseGenerator = default_random_engine();
    unsigned int runsCount = 10;
    unsigned int solutionsCount = 10;
    double closeOrLongRangeOperatorParameter = 0.7;
    double variantCloseNormalLongCauchySum = 0.0, variantCloseNormalLongNormalSum = 0.0, variantCloseUniformLongCauchySum = 0.0;
    NormalRangeOperator closeNormalOperator = NormalRangeOperator(0.0, 1.0);
    NormalRangeOperator longNormalOperator = NormalRangeOperator(0.0, 1000.0);
    UniformRangeOperator uniformOperator = UniformRangeOperator(-1.0, 1.0);
    CauchyRangeOperator cauchyOperator = CauchyRangeOperator(100.0, 0.0);
    for (int runsIdx = 0; runsIdx < runsCount; runsIdx++)
    {
        vector<double> closeNormalLongCauchy = multipleWorlds2ndVariant(baseGenerator , dimensionality, solutionsCount, continueCondition1, ackleyFunOptimizationType, ackleyFunction, closeNormalOperator, cauchyOperator, closeOrLongRangeOperatorParameter);
        cout << "*";
        vector<double> closeNormalLongNormal = multipleWorlds2ndVariant(baseGenerator, dimensionality, solutionsCount, continueCondition1 , ackleyFunOptimizationType, ackleyFunction, closeNormalOperator, longNormalOperator, closeOrLongRangeOperatorParameter);
        cout << "#";
        vector<double> closeUniformLongCauchy = multipleWorlds2ndVariant(baseGenerator ,dimensionality, solutionsCount, continueCondition1 ,ackleyFunOptimizationType, ackleyFunction, uniformOperator, cauchyOperator, closeOrLongRangeOperatorParameter);
        cout << "$" << endl ;
        variantCloseNormalLongCauchySum += ackleyFunction(closeNormalLongCauchy);
        variantCloseNormalLongNormalSum += ackleyFunction(closeNormalLongNormal);
        variantCloseUniformLongCauchySum += ackleyFunction(closeUniformLongCauchy);
    }
    variantCloseNormalLongCauchySum /= static_cast<double>(runsCount);
    variantCloseNormalLongNormalSum /= static_cast<double>(runsCount);
    variantCloseUniformLongCauchySum /= static_cast<double>(runsCount);
    cout << "Average Ackley function value for " << runsCount << " runs using multple worlds algorithm with Normal distribution as close operator and Cauchy distribution as long operator: " << variantCloseNormalLongCauchySum << endl;
    cout << "Average Ackley function value for " << runsCount << " runs using multple worlds algorithm with Normal distribution as close operator and Normal distribution as long operator: " << variantCloseNormalLongNormalSum << endl;
    cout << "Average Ackley function value for " << runsCount << " runs using multple worlds algorithm with Uniform distribution as close operator and Cauchy distribution as long operator: " << variantCloseUniformLongCauchySum << endl;

    return 0;
}




void testDistributionsWithPassedGeneratorByRef(default_random_engine& generator)
{
    uniform_real_distribution<double> distrUniform = uniform_real_distribution<double>();
    cout << "oooo" << endl;
    cout << distrUniform(generator) << endl;
    cout << distrUniform(generator) << endl;
    cout << "ooaaoo" << endl;
}
void test()
{
    cout << "test uniform real distribution" << endl;
    default_random_engine baseGenerator = default_random_engine();
    uniform_real_distribution<double> distrUniform = uniform_real_distribution<double>();
    for (int i = 0; i < 5; i++)
    {
        testDistributionsWithPassedGeneratorByRef(baseGenerator);
    }
    cout << "test uniform real distribution done" << endl;
    unsigned int dimensionality = 4;
    unsigned int decimalPlaces = 20;
    NormalRangeOperator closeNormalOperator = NormalRangeOperator(0.0, 1.0, baseGenerator);
    NormalRangeOperator longNormalOperator = NormalRangeOperator(0.0, 1000.0, baseGenerator);
    UniformRangeOperator uniformOperator = UniformRangeOperator(-1.0, 1.0, baseGenerator);
    CauchyRangeOperator cauchyOperator = CauchyRangeOperator(100.0, 0.0, baseGenerator);
    vector<double> currentPoint = initWithUniformRandomValues(dimensionality, baseGenerator);
    vector<double> currentPoint2 = initWithUniformRandomValues(dimensionality, baseGenerator);
    cout << "xyz" << endl;
    printVector(currentPoint, decimalPlaces);
    cout << endl;
    printVector(currentPoint2, decimalPlaces);
    cout << endl;
    cout << "vectors equal: " << (currentPoint == currentPoint2) << " , same compared: " << (currentPoint == currentPoint) << endl;
    cout << "XYZ" << endl;
    cout << "======== start test: ========\n";
    cout << "start value: \n";
    printVector(currentPoint, decimalPlaces);
    cout << "\n";


    cout << "Cauchy:" << endl;
    bool vectorsChange = true;
    vector<double> previous = currentPoint;
    for (int i = 0; i < 10; i++)
    {
        currentPoint = cauchyOperator(currentPoint);
        vectorsChange = vectorsChange && (currentPoint != previous);
        previous = currentPoint;
        printVector(currentPoint, decimalPlaces);
        cout << "\n";
    }
    cout << "vectros change: " << vectorsChange << endl;



    vectorsChange = true;
    cout << "Normal close:" << endl;
    currentPoint = initWithUniformRandomValues(dimensionality, baseGenerator);
    previous = currentPoint;
    cout << "start value: \n";
    printVector(currentPoint, decimalPlaces);
    cout << "\n";
    for (int i = 0; i < 10; i++)
    {
        currentPoint = closeNormalOperator(currentPoint);
        vectorsChange = vectorsChange && (currentPoint != previous);
        cout << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa " << (currentPoint == previous) << endl;
        previous = currentPoint;
        printVector(currentPoint, decimalPlaces);
        cout << "\n";
    }
    cout << "vectros change: " << vectorsChange << endl;
    vectorsChange = true;
    cout << "Normal long:" << endl;
    currentPoint = initWithUniformRandomValues(dimensionality, baseGenerator);
    previous = currentPoint;
    cout << "start value: \n";
    printVector(currentPoint, decimalPlaces);
    cout << "\n";
    for (int i = 0; i < 10; i++)
    {
        currentPoint = longNormalOperator(currentPoint);
        vectorsChange = vectorsChange && (currentPoint != previous);
        previous = currentPoint;
        printVector(currentPoint, decimalPlaces);
        cout << "\n";
    }
    cout << "vectros change: " << vectorsChange << endl;
    vectorsChange = true;
    cout << "Uniform:" << endl;
    currentPoint = initWithUniformRandomValues(dimensionality, baseGenerator);
    previous = currentPoint;
    cout << "start value: \n";
    printVector(currentPoint, decimalPlaces);
    cout << "\n";
    for (int i = 0; i < 10; i++)
    {
        currentPoint = uniformOperator(currentPoint);
        vectorsChange = vectorsChange && (currentPoint != previous);
        previous = currentPoint;
        printVector(currentPoint, decimalPlaces);
        cout << "\n";
    }
    cout << "vectros change: " << vectorsChange << endl;
    vectorsChange = true;
    cout << "======== finish test: ========" << endl;
}
