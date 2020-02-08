#include "GameState.hpp"

namespace
{
constexpr int rowsCount()
{
    static_assert(BOARD_SIZE == 8, "Not handled board size");
    return 3;
}
} // namespace

GameState::GameState()
{
    m_gameState = Board{};
    for (int row = 0; row < rowsCount(); row++)
    {
        for (int col = 0; col < BOARD_SIZE; col += 2)
        {
            m_gameState[row][col + row % 2] = FigureState{FigureColor::White};
            m_gameState[BOARD_SIZE - row - 1][col + (row + 1) % 2] = FigureState{FigureColor::Black};
        }
    }
}

GameState::GameState(Board&& board) : m_gameState(board) {}

void GameState::removePawn(const Position& position)
{
    m_gameState[position.row][position.col] = std::nullopt;
}

void GameState::movePawn(const Position& from, const Position& to)
{
    m_gameState[to.row][to.col] = m_gameState[from.row][from.col];
    removePawn(from);
}

void GameState::changePawnType(const Position& position, FigureType type)
{
    m_gameState[position.row][position.col]->type = type;
}

bool GameState::isFree(const Position& position) const
{
    return !m_gameState[position.row][position.col].has_value();
}

bool GameState::isValid(const Position& position)
{
    const bool rowValid = (position.row >= 0 && position.row < BOARD_SIZE);
    const bool colValid = (position.col >= 0 && position.col < BOARD_SIZE);
    return rowValid && colValid && (position.col % 2 == (position.row % 2 == 0 ? 0 : 1));
}

FigureState GameState::pawnAtPosition(const Position& position) const
{
    return m_gameState[position.row][position.col].value();
}

Figures GameState::pawns(FigureColor color) const
{
    Figures ret;
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0 + row % 2; col < BOARD_SIZE; col += 2)
        {
            if (m_gameState[row][col] && m_gameState[row][col]->color == color)
            {
                ret.push_back({m_gameState[row][col].value(), {row, col}});
            }
        }
    }
    return ret;
}

bool GameState::operator==(const GameState& other) const
{
    return m_gameState == other.m_gameState;
}
