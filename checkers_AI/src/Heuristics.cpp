#include "Heuristics.hpp"
#include "IStrategy.hpp"

Heuristics::Heuristics(
    const IStrategy& strategy,
    const IMetricsCalculator& metricsCalculator,
    MetricsWithFactors metricsEarlyGame,
    MetricsWithFactors metricsMidGame,
    MetricsWithFactors metricsLateGame,
    int earlyGameFiguresLimit,
    int midGameFiguresLimit,
    int minimaxDepth)
    : m_strategy{strategy}
    , m_metricsCalculator{metricsCalculator}
    , m_metricsEarlyGame{std::move(metricsEarlyGame)}
    , m_metricsMidGame{std::move(metricsMidGame)}
    , m_metricsLateGame{std::move(metricsLateGame)}
    , m_earlyGameFiguresLimit{earlyGameFiguresLimit}
    , m_midGameFiguresLimit{midGameFiguresLimit}
    , m_minimaxDepth{minimaxDepth}
{
}

bool Heuristics::operator==(const Heuristics& other) const
{
    return m_metricsEarlyGame == other.m_metricsEarlyGame && m_metricsEarlyGame == other.m_metricsEarlyGame &&
        m_metricsEarlyGame == other.m_metricsEarlyGame && m_metricsEarlyGame == other.m_metricsEarlyGame &&
        m_metricsEarlyGame == other.m_metricsEarlyGame && m_metricsEarlyGame == other.m_metricsEarlyGame;
}

GameStateWithMove Heuristics::getMove(const GameState& gameState, FigureColor figureColor)
{
    const auto whiteFiguresCount = gameState.pawns(FigureColor::White).size();
    const auto blackFiguresCount = gameState.pawns(FigureColor::Black).size();
    const auto totalFiguresCount = whiteFiguresCount + blackFiguresCount;
    if (totalFiguresCount >= m_earlyGameFiguresLimit)
    {
        return m_strategy.getMiniMaxMove(
            gameState,
            [this](const GameState& gameState, FigureColor figureColor) {
                return m_metricsCalculator.evaluate(m_metricsEarlyGame, gameState, figureColor);
            },
            figureColor,
            m_minimaxDepth);
    }
    else if (totalFiguresCount >= m_midGameFiguresLimit)
    {
        return m_strategy.getMiniMaxMove(
            gameState,
            [this](const GameState& gameState, FigureColor figureColor) {
                return m_metricsCalculator.evaluate(m_metricsMidGame, gameState, figureColor);
            },
            figureColor,
            m_minimaxDepth);
    }
    else
    {
        return m_strategy.getMiniMaxMove(
            gameState,
            [this](const GameState& gameState, FigureColor figureColor) {
                return m_metricsCalculator.evaluate(m_metricsLateGame, gameState, figureColor);
            },
            figureColor,
            m_minimaxDepth);
    }
}
