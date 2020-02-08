#pragma once

#include "IMetricsCalculator.hpp"

struct Metrics
{
    MetricsWithFactors metricsEarlyGame;
    MetricsWithFactors metricsMidGame;
    MetricsWithFactors metricsLateGame;
};

namespace File
{
Metrics deserialize(const std::string& filename);
}
