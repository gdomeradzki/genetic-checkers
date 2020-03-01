#include "GamePlay.hpp"
#include <algorithm>

bool operator==(const GameStateWithMove& first, const GameStateWithMove& second)
{
    return first.gameState == second.gameState;
}

GamePlay::GamePlay(GameState& gameState, MoveDecisionCallback whiteStrategy, MoveDecisionCallback blackStrategy)
    : currentGameState(gameState), whiteStrategy(std::move(whiteStrategy)), blackStrategy(std::move(blackStrategy))
{
}

GameResult GamePlay::start()
{
    constexpr auto movesWithNoBeatWhichMakesDraw = 20;
    FigureColor currentColor{FigureColor::White};
    int movesWithNoBeats = 0;
    m_gameplayInterrupted = false;
    while (!m_gameplayInterrupted)
    {
        GameController gameController(currentGameState);
        GameStateWithMove decision;
        std::vector<GameStateWithMove> possibleMoves;
        possibleMoves = gameController.getPossibleMoves(currentColor);
        if (possibleMoves.empty())
        {
            m_gameplayInterrupted = true;
            return currentColor == FigureColor::White ? GameResult::BlackWin : GameResult::WhiteWin;
        }

        if (currentColor == FigureColor::White)
        {
            decision = whiteStrategy(currentGameState, possibleMoves);
        }
        else
        {
            decision = blackStrategy(currentGameState, possibleMoves);
        }
        currentColor = FigureState::flipColor(currentColor);
        if (currentGameState.pawns(currentColor).size() == decision.gameState.pawns(currentColor).size())
        {
            movesWithNoBeats++;
        }
        else
        {
            movesWithNoBeats = 0;
        }
        if (std::find(possibleMoves.cbegin(), possibleMoves.cend(), decision) == possibleMoves.end())
        {
            m_gameplayInterrupted = true;
            throw std::runtime_error("Move not allowed");
        }
        if (movesWithNoBeats == movesWithNoBeatWhichMakesDraw)
        {
            m_gameplayInterrupted = true;
            return GameResult::Draw;
        }
        currentGameState = decision.gameState;
    }
    return GameResult::GameOn;
}

void GamePlay::stopGameplay()
{
    m_gameplayInterrupted = true;
}
