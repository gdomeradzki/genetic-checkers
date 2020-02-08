#pragma once

#include "IMetricsCalculator.hpp"

namespace Calculator
{
constexpr auto minValue = 0;
constexpr auto maxValue = 100;
constexpr auto valueRange = maxValue - minValue;
} // namespace Calculator
class MetricsCalculator : public IMetricsCalculator
{
public:
    int evaluate(const MetricsWithFactors& metricWithFactors, const GameState&, FigureColor) const override;
};
