#include "GameState.hpp"

namespace
{
constexpr int rowsCount()
{
    static_assert(boardSize == 8, "Not handled board size");
    return 3;
}
} // namespace

GameState::GameState()
{
    gameState = Board{};
    for (int row = 0; row < rowsCount(); row++)
    {
        for (auto col = 0u; col < boardSize; col += 2)
        {
            gameState[row][col + row % 2] = FigureState{FigureColor::White};
            gameState[boardSize - row - 1][col + (row + 1) % 2] = FigureState{FigureColor::Black};
        }
    }
}

GameState::GameState(Board&& board) : gameState(board) {}

void GameState::removePawn(const Position& position)
{
    gameState[position.row][position.col] = std::nullopt;
}

void GameState::movePawn(const Position& from, const Position& to)
{
    gameState[to.row][to.col] = gameState[from.row][from.col];
    removePawn(from);
}

void GameState::changePawnType(const Position& position, FigureType type)
{
    gameState[position.row][position.col]->type = type;
}

bool GameState::isFree(const Position& position) const
{
    return !gameState[position.row][position.col].has_value();
}

bool GameState::isValid(const Position& position)
{
    const bool rowValid = (position.row >= 0 && position.row < boardSize);
    const bool colValid = (position.col >= 0 && position.col < boardSize);
    return rowValid && colValid && (position.col % 2 == (position.row % 2 == 0 ? 0 : 1));
}

FigureState GameState::pawnAtPosition(const Position& position) const
{
    return gameState[position.row][position.col].value();
}

Figures GameState::pawns(FigureColor color) const
{
    Figures ret;
    for (int row = 0; row < boardSize; row++)
    {
        for (int col = 0 + row % 2; col < boardSize; col += 2)
        {
            if (gameState[row][col] && gameState[row][col]->color == color)
            {
                ret.push_back({gameState[row][col].value(), {row, col}});
            }
        }
    }
    return ret;
}

bool GameState::operator==(const GameState& other) const
{
    return gameState == other.gameState;
}
