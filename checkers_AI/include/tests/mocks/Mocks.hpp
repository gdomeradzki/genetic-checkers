#include <gmock/gmock.h>

#include "MetricsCalculator.hpp"
#include "Strategy.hpp"
class StrategyMock : public IStrategy
{
public:
    // clang-format off
    MOCK_METHOD(GameStateWithMove, getMiniMaxMove, (const GameState&, EvaluationFunction, FigureColor, int), (const, override));
    // clang-format on
    MOCK_METHOD(GameStateWithMove, getRandomMove, (const std::vector<GameStateWithMove>&), (const, override));
};
class CalculatorMock : public IMetricsCalculator
{
public:
    MOCK_METHOD(
        int,
        evaluate,
        (const MetricsWithFactors& metricWithFactors, const GameState&, FigureColor),
        (const, override));
};
