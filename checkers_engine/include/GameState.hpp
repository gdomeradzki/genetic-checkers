#pragma once

#include <array>
#include <optional>
#include <utility>
#include <vector>
#include "PawnState.hpp"
#include "Types.hpp"

constexpr auto BOARD_SIZE = 8;
constexpr auto totalPlayerFiguresNumber = BOARD_SIZE * (BOARD_SIZE - 2) / 4;

struct Figure
{
    FigureState state;
    Position position;
};

using Board = std::array<std::array<std::optional<FigureState>, BOARD_SIZE>, BOARD_SIZE>;
using Figures = std::vector<Figure>;
class GameState
{
public:
    GameState();
    GameState(Board&& board);
    void removePawn(const Position&);
    void movePawn(const Position&, const Position&);
    void changePawnType(const Position&, FigureType);
    bool isFree(const Position&) const;
    static bool isValid(const Position&);
    FigureState pawnAtPosition(const Position&) const;
    Figures pawns(FigureColor) const;

    bool operator==(const GameState&) const;

private:
    Board m_gameState;
};
