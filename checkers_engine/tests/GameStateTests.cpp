#include <gtest/gtest.h>

#include "GameState.hpp"

TEST(GameState, ShouldHaveBoardDefaultContructedWhitePawns)
{
    GameState state;
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 8; col += 2)
        {
            Position whitePawnPosition{row, col + row % 2};
            const auto pawn = state.pawnAtPosition(whitePawnPosition);
            EXPECT_EQ(pawn.color, FigureColor::White);
            EXPECT_EQ(pawn.type, FigureType::Pawn);
        }
    }
}
TEST(GameState, ShouldHaveBoardDefaultContructedBlackPawns)
{
    GameState state;
    for (int row = BOARD_SIZE - 1; row >= BOARD_SIZE - 3; row--)
    {
        for (int col = 0; col < 8; col += 2)
        {
            Position blackPawnPosition{row, col + (row) % 2};
            const auto pawn = state.pawnAtPosition(blackPawnPosition);
            EXPECT_EQ(pawn.color, FigureColor::Black);
            EXPECT_EQ(pawn.type, FigureType::Pawn);
        }
    }
}
TEST(GameState, ShouldHaveBoardDefaultContructedEmptyFields)
{
    GameState state;
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 8; col += 2)
        {
            Position emptyFieldPosition{row, col + (row + 1) % 2};
            EXPECT_TRUE(state.isFree(emptyFieldPosition));
        }
    }

    for (int row = BOARD_SIZE - 1; row >= BOARD_SIZE - 3; row--)
    {
        for (int col = 0; col < 8; col += 2)
        {
            Position emptyFieldPosition{row, col + (row + 1) % 2};
            EXPECT_TRUE(state.isFree(emptyFieldPosition));
        }
    }
}

TEST(GameState, ShouldRemovePawn)
{
    GameState state;
    const auto pos1 = Position{0, 0};
    const auto pos2 = Position{7, 7};
    const auto pos3 = Position{1, 1};
    EXPECT_FALSE(state.isFree(pos1));
    EXPECT_FALSE(state.isFree(pos2));
    EXPECT_FALSE(state.isFree(pos3));
    state.removePawn(pos1);
    state.removePawn(pos2);
    state.removePawn(pos3);
    EXPECT_TRUE(state.isFree(pos1));
    EXPECT_TRUE(state.isFree(pos2));
    EXPECT_TRUE(state.isFree(pos3));
}

TEST(GameState, ShouldMovePawn)
{
    GameState state;
    const auto from = Position{0, 0};
    const auto to = Position{7, 7};
    EXPECT_FALSE(state.isFree(from));
    EXPECT_FALSE(state.isFree(to));
    const auto pawn1 = state.pawnAtPosition(from);
    auto pawn2 = state.pawnAtPosition(to);
    EXPECT_EQ(pawn1.color, FigureColor::White);
    EXPECT_EQ(pawn2.color, FigureColor::Black);
    state.movePawn(from, to);
    EXPECT_TRUE(state.isFree(from));
    EXPECT_FALSE(state.isFree(to));
    pawn2 = state.pawnAtPosition(to);
    EXPECT_EQ(pawn2.color, FigureColor::White);
}

TEST(GameState, ShouldReturnAllPawnsByColor)
{
    GameState state;
    const auto from = Position{0, 0};
    const auto to = Position{7, 7};
    state.movePawn(from, to);
    const auto whitePawns = state.pawns(FigureColor::White);
    const auto blackPawns = state.pawns(FigureColor::Black);
    EXPECT_EQ(whitePawns.size(), 12);
    EXPECT_EQ(blackPawns.size(), 11);
}
TEST(GameState, isValidPosition)
{
    EXPECT_TRUE(GameState::isValid({0, 0}));
    EXPECT_TRUE(GameState::isValid({1, 1}));
    EXPECT_TRUE(GameState::isValid({7, 7}));
    EXPECT_TRUE(GameState::isValid({0, 6}));
    EXPECT_TRUE(GameState::isValid({7, 1}));

    EXPECT_FALSE(GameState::isValid({-1, 0}));
    EXPECT_FALSE(GameState::isValid({1, -1}));
    EXPECT_FALSE(GameState::isValid({-1, -1}));
    EXPECT_FALSE(GameState::isValid({8, 6}));
    EXPECT_FALSE(GameState::isValid({7, 8}));

    EXPECT_FALSE(GameState::isValid({0, 1}));
    EXPECT_FALSE(GameState::isValid({7, 0}));
    EXPECT_FALSE(GameState::isValid({5, 4}));
}
