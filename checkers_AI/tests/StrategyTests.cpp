#include <gtest/gtest.h>

#include "Strategy.hpp"
struct StrategyTest : public ::testing::Test
{
    StrategyTest() : sut{std::make_unique<Strategy>()} {}
    std::unique_ptr<IStrategy> sut;
};

TEST_F(StrategyTest, AlphaBetaPrunningShouldEmptyMoveWhenNoPossibleMove)
{
    Board board;
    board[1][7] = FigureState{FigureColor::Black};
    board[0][6] = FigureState{FigureColor::White};
    GameState gameState{std::move(board)};

    const auto result = sut->getMiniMaxMove(
        gameState, [](const GameState&, FigureColor) { return 1; }, FigureColor::Black, 15);
    EXPECT_TRUE(result.move.empty());
}
TEST_F(StrategyTest, AlphaBetaPrunningShouldEmptyMoveWhenMaxDepthIs0)
{
    Board board;
    board[1][7] = FigureState{FigureColor::Black};
    board[1][3] = FigureState{FigureColor::Black};
    board[0][6] = FigureState{FigureColor::White};
    GameState gameState{std::move(board)};

    const auto result = sut->getMiniMaxMove(
        gameState, [](const GameState&, FigureColor) { return 1; }, FigureColor::Black, 0);
    EXPECT_TRUE(result.move.empty());
}

TEST_F(StrategyTest, AlphaBetaPrunningShouldReturnAlwaysFirstMoveOfRequestingPlayer)
{
    Board board;
    board[0][4] = FigureState{FigureColor::White};
    board[7][3] = FigureState{FigureColor::Black};
    GameState gameState{std::move(board)};

    for (int i = 1; i < 5; i++)
    {
        const auto result = sut->getMiniMaxMove(
            gameState, [](const GameState&, FigureColor) { return 1; }, FigureColor::White, i);
        const auto& move = result.move;
        EXPECT_EQ(move.size(), 2);
        EXPECT_EQ(move.at(0).row, 0);
        EXPECT_EQ(move.at(0).col, 4);
        EXPECT_EQ(move.at(1).row, 1);
    }
}

TEST_F(StrategyTest, AlphaBetaPrunningShouldReturnConcreteMoveAndDoPrunning)
{
    constexpr auto failureEvalValue = 99999;
    auto getExpectationsOnConcreteEvaluationAndReturnValue = [](int callNumber, FigureColor player) {
        EXPECT_EQ(player, FigureColor::White);
        switch (callNumber)
        {
            case 0:
                return 3;
            case 1:
                return 5;
            case 2:
                return 6;
            case 3:
                return 1;
            case 4:
                return 2;
            default:
                return failureEvalValue;
        }
    };
    Board board;
    board[0][4] = FigureState{FigureColor::White};
    board[7][3] = FigureState{FigureColor::Black};
    GameState gameState{std::move(board)};
    int callNumber = 0;
    const auto returnedGameState = sut->getMiniMaxMove(
        gameState,
        [&](const GameState&, FigureColor player) {
            int evalValue = getExpectationsOnConcreteEvaluationAndReturnValue(callNumber, player);
            EXPECT_NE(evalValue, failureEvalValue);
            callNumber++;
            return evalValue;
        },
        FigureColor::White,
        3);
    EXPECT_EQ(callNumber, 5);
    const auto& move = returnedGameState.move;
    EXPECT_EQ(move.size(), 2);
    EXPECT_EQ(move.at(0).row, 0);
    EXPECT_EQ(move.at(0).col, 4);
    EXPECT_EQ(move.at(1).row, 1);
    EXPECT_EQ(move.at(1).col, 3);
}
