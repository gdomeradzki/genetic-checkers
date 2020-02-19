#pragma once

#include <set>
#include "GameState.hpp"

enum class Metric : unsigned int
{
    PawnsNumber,
    KingsNumber,
    SafePawns,
    SafeKings,
    MoveablePawns,
    MoveableKings,
    AggregatedDistanceToPromotionLine,
    NumberOfUnoccupiedFieldOnPromotionLine,
    DefenderFigures,
    AttackingFigures,
    CenterPawns,
    CenterKings,
    PawnsOnDiagonal,
    KingsOnDiagonal,
    PawnsOnDoubleDiagonal,
    KingsOnDoubleDiagonal,
    Triangle,
    Oreo,
    Bridge,
    Corner,
    KingCorner,
    Dog
};
struct MetricFactor
{
    explicit MetricFactor(Metric metric) : metric{metric} {}
    MetricFactor(Metric metric, unsigned int factor) : metric{metric}, factor{factor} {}
    Metric metric;
    unsigned int factor{1};
    bool operator<(const MetricFactor& other) const { return metric < other.metric; }
    bool operator==(const MetricFactor& other) const { return metric == other.metric && factor == other.factor; }
};

using MetricsWithFactors = std::set<MetricFactor>;
class IMetricsCalculator
{
public:
    virtual ~IMetricsCalculator() = default;
    virtual int evaluate(const MetricsWithFactors& metricWithFactors, const GameState&, FigureColor) const = 0;
};
