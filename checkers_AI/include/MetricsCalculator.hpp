#pragma once

#include "IMetricsCalculator.hpp"

namespace Calculator
{
constexpr auto minValue = 0u;
constexpr auto maxValue = 100u;
constexpr auto valueRange = maxValue - minValue;
} // namespace Calculator
class MetricsCalculator : public IMetricsCalculator
{
public:
    unsigned int evaluate(const MetricsWithFactors& metricWithFactors, const GameState&, FigureColor) const override;
};
