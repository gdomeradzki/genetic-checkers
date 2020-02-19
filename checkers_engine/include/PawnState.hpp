#pragma once

enum class FigureType
{
    King,
    Pawn
};

enum class FigureColor
{
    White,
    Black
};

namespace utils
{
} // namespace utils
struct FigureState
{
    FigureState() : type(FigureType::Pawn), color(FigureColor::White) {}
    explicit FigureState(FigureColor color) : type(FigureType::Pawn), color(color) {}
    FigureState(FigureType type, FigureColor color) : type(type), color(color) {}

    static FigureColor flipColor(FigureColor color)
    {
        return color == FigureColor::White ? FigureColor::Black : FigureColor::White;
    }
    bool operator==(const FigureState& other) const { return color == other.color && type == other.type; }
    FigureType type;
    FigureColor color;
};
