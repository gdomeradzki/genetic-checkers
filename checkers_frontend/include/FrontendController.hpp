#pragma once

#include <memory>

#include <QEventLoop>

#include "FrontBoard.hpp"
#include "GamePlay.hpp"
#include "Heuristics.hpp"
#include "MainWindow.hpp"
#include "MetricsCalculator.hpp"
#include "Strategy.hpp"

class FrontendController
{
public:
    FrontendController();

private:
    void startNewGame();

    void finishGame(GameResult);
    void loadAI(const std::string& fileName);

    MoveDecisionCallback getHumanDecisionCallback();
    Move lastHumanMove;

    std::unique_ptr<GamePlay> gamePlay;
    GameState gameState;
    MainWindow mainWindow;
    QEventLoop humanMovesEventLoop;

    Strategy strategy;
    MetricsCalculator metricsCalculator;
    std::unique_ptr<Heuristics> ai;
};
