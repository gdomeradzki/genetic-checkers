#pragma once

#include <vector>
#include "GameState.hpp"
#include "Types.hpp"

struct GameStateWithMove
{
    GameState gameState;
    Move move;
};

class GameController
{
public:
    GameController(const GameState&);

    std::vector<GameStateWithMove> getPossibleMoves(FigureColor) const;

    GameResult gameResult(FigureColor) const;

private:
    std::vector<GameStateWithMove> getAvailableJumps(const Figure&) const;

    std::vector<GameStateWithMove> getAvailableMoves(const Figure&) const;

    void findJump(GameState, Move, const Figure, std::vector<GameStateWithMove>&) const;

    bool isKingChange(FigureState, Position) const;

    const GameState& m_gameState;
};
