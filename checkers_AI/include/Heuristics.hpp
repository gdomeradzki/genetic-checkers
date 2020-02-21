#pragma once

#include <vector>
#include "GameController.hpp"
#include "IMetricsCalculator.hpp"

class IStrategy;
class Heuristics
{
public:
    Heuristics() = delete;
    Heuristics(
        const IStrategy& strategy,
        const IMetricsCalculator& metricsCalculator,
        MetricsWithFactors metricsEarlyGame,
        MetricsWithFactors metricsMidGame,
        MetricsWithFactors metricsLateGame,
        unsigned int earlyGameFiguresLimit,
        unsigned int midGameFiguresLimit,
        unsigned int minimaxDepth);

    ~Heuristics() = default;

    Heuristics(Heuristics&&) = default;
    Heuristics(const Heuristics&) = default;

    Heuristics& operator=(Heuristics&&) = delete;
    Heuristics& operator=(const Heuristics&) = delete;

    bool operator==(const Heuristics&) const;

    GameStateWithMove getMove(const GameState&, FigureColor);

private:
    const IStrategy& m_strategy;
    const IMetricsCalculator& m_metricsCalculator;
    const MetricsWithFactors m_metricsEarlyGame;
    const MetricsWithFactors m_metricsMidGame;
    const MetricsWithFactors m_metricsLateGame;
    const unsigned int m_earlyGameFiguresLimit;
    const unsigned int m_midGameFiguresLimit;
    const unsigned int m_minimaxDepth;
};
