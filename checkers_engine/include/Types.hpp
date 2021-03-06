#pragma once

#include <vector>

struct Position
{
    int row{0};
    int col{0};
    bool operator<(const Position& other) const
    {
        if (row == other.row)
        {
            return col < other.col;
        }
        return row < other.row;
    }
    bool operator==(const Position& other) const { return row == other.row && col == other.col; }
};

using Move = std::vector<Position>;

enum class GameResult
{
    WhiteWin,
    BlackWin,
    GameOn,
    Draw
};
