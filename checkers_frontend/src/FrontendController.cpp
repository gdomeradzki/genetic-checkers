#include "FrontendController.hpp"
#include <QApplication>
#include <algorithm>

#include "Helpers.hpp"

#include <QDebug>
FrontendController::FrontendController() : mainWindow{gameState}
{
    mainWindow.show();
    mainWindow.blockPawnMoves(FigureColor::Black, true);
    mainWindow.blockPawnMoves(FigureColor::White, true);
    QObject::connect(&mainWindow, &MainWindow::signalMoveDone, [this](const Move& move) {
        lastHumanMove = move;
        humanMovesEventLoop.quit();
    });
    QObject::connect(&mainWindow, &MainWindow::signalStartNewGame, [this]() { startNewGame(); });
    QObject::connect(
        &mainWindow, &MainWindow::signalLoadMetrics, [this](const std::string& filename) { loadAI(filename); });
}

void FrontendController::startNewGame()
{
    gameState = GameState{};
    mainWindow.syncTemporaryState();

    MoveDecisionCallback whiteStrategy;
    MoveDecisionCallback blackStrategy;
    QString whiteStrategyText = " (white)";
    QString blackStrategyText = " (black)";
    switch (mainWindow.getBlackStrategy())
    {
        case StrategyType::Human:
            blackStrategy = getHumanDecisionCallback();
            mainWindow.blockPawnMoves(FigureColor::Black, false);
            blackStrategyText.prepend("Human");
            break;
        case StrategyType::Ai:
            blackStrategyText.prepend("AI");
            blackStrategy =
                [this](const InitialGameState& initalGameState, const std::vector<GameStateWithMove>& possibleMoves) {
                    if (ai)
                    {
                        return ai->getMove(initalGameState, FigureColor::Black);
                    }
                    return possibleMoves.front();
                };
            break;
    }
    switch (mainWindow.getWhiteStrategy())
    {
        case StrategyType::Human:
            whiteStrategy = getHumanDecisionCallback();
            mainWindow.blockPawnMoves(FigureColor::White, false);
            whiteStrategyText.prepend("Human");
            break;
        case StrategyType::Ai:
            whiteStrategyText.prepend("AI");
            whiteStrategy =
                [this](const InitialGameState& initalGameState, const std::vector<GameStateWithMove>& possibleMoves) {
                    if (ai)
                    {
                        return ai->getMove(initalGameState, FigureColor::White);
                    }
                    return possibleMoves.front();
                };
            break;
    }
    mainWindow.pushInfo(QString("New game start:") + whiteStrategyText + QString(" vs ") + blackStrategyText);

    gamePlay = std::make_unique<GamePlay>(gameState, whiteStrategy, blackStrategy);
    const auto gameResult = gamePlay->start();
    finishGame(gameResult);
}

void FrontendController::finishGame(GameResult gameResult)
{
    mainWindow.blockPawnMoves(FigureColor::Black, true);
    mainWindow.blockPawnMoves(FigureColor::White, true);

    gameState = GameState{};
    mainWindow.showGameResultStatement(gameResult);
}

void FrontendController::loadAI(const std::string& fileName)
{
    Metrics metrics;
    try
    {
        metrics = File::deserialize(fileName);
    }
    catch (const std::exception& e)
    {
        mainWindow.pushInfo(e.what());
        return;
    }
    ai.reset(new Heuristics{strategy,
                            metricsCalculator,
                            std::move(metrics.metricsEarlyGame),
                            std::move(metrics.metricsMidGame),
                            std::move(metrics.metricsLateGame),
                            totalPlayerFiguresNumber / 3,
                            2 * totalPlayerFiguresNumber / 3,
                            mainWindow.minimaxDeep()});
    mainWindow.pushInfo(QString("File loaded: ") + QString::fromStdString(fileName));
}
MoveDecisionCallback FrontendController::getHumanDecisionCallback()
{
    return [this](const InitialGameState&, const std::vector<GameStateWithMove>& possibleMoves) {
        mainWindow.syncTemporaryState();
        std::vector<GameStateWithMove> movesToCheck;
        while (true)
        {
            movesToCheck = possibleMoves;
            do
            {
                mainWindow.pushInfo("Waiting for human player to make a move...");
                constexpr auto eventLoopSuccessExitCode = 0;
                if (humanMovesEventLoop.exec() != eventLoopSuccessExitCode)
                {
                    gamePlay->stopGameplay();
                    return possibleMoves.front();
                }
                for (uint jumpNumber = 0; jumpNumber < lastHumanMove.size(); jumpNumber++)
                {
                    movesToCheck.erase(
                        std::remove_if(
                            movesToCheck.begin(),
                            movesToCheck.end(),
                            [this, jumpNumber](const auto& gameStateWithMove) {
                                return !(gameStateWithMove.move.at(jumpNumber) == lastHumanMove.at(jumpNumber));
                            }),
                        movesToCheck.end());
                }

            } while (movesToCheck.size() > 1);

            if (!movesToCheck.empty())
            {
                mainWindow.pushInfo("Requested move ok");
                break;
            }
            mainWindow.pushInfo("Requested move not allowed");
            mainWindow.syncTemporaryState();
        }

        return movesToCheck.front();
    };
}
