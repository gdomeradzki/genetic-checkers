#include <gmock/gmock.h>

#include "MetricsCalculator.hpp"
#include "Strategy.hpp"
class StrategyMock : public IStrategy
{
public:
    MOCK_METHOD(
        GameStateWithMove,
        getMiniMaxMove,
        (const GameState&, EvaluationFunction, FigureColor, unsigned int),
        (const, override));
};
class CalculatorMock : public IMetricsCalculator
{
public:
    MOCK_METHOD(
        unsigned int,
        evaluate,
        (const MetricsWithFactors& metricWithFactors, const GameState&, FigureColor),
        (const, override));
};
