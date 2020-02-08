#pragma once
#include <functional>
#include "GameController.hpp"

using InitialGameState = GameState;
using MoveDecisionCallback =
    std::function<GameStateWithMove(const InitialGameState&, const std::vector<GameStateWithMove>&)>;

class GamePlay
{
public:
    GamePlay(GameState& gameState, MoveDecisionCallback whiteStrategy, MoveDecisionCallback blackStrategy);

    GameResult start();
    void stopGameplay();

private:
    bool m_gameplayInterrupted{true};

    GameState& currentGameState;
    MoveDecisionCallback whiteStrategy;
    MoveDecisionCallback blackStrategy;
};
