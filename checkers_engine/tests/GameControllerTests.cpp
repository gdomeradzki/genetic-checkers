#include <gtest/gtest.h>

#include "GameController.hpp"

TEST(GameController, DefaultState)
{
    GameState state;
    GameController controller(state);
    EXPECT_EQ(controller.gameResult(FigureColor::White), GameResult::GameOn);
    EXPECT_EQ(controller.gameResult(FigureColor::Black), GameResult::GameOn);

    const auto possibleWhiteMoves = controller.getPossibleMoves(FigureColor::White);
    const auto possibleBlackMoves = controller.getPossibleMoves(FigureColor::Black);

    EXPECT_EQ(possibleWhiteMoves.size(), 7);
    EXPECT_EQ(possibleBlackMoves.size(), 7);

    for (const auto& possibility : possibleWhiteMoves)
    {
        EXPECT_EQ(possibility.gameState.pawns(FigureColor::White).size(), 12);
        EXPECT_EQ(possibility.gameState.pawns(FigureColor::Black).size(), 12);
        EXPECT_EQ(possibility.move.size(), 2);
    }
    for (const auto& possibility : possibleBlackMoves)
    {
        EXPECT_EQ(possibility.gameState.pawns(FigureColor::White).size(), 12);
        EXPECT_EQ(possibility.gameState.pawns(FigureColor::Black).size(), 12);
        EXPECT_EQ(possibility.move.size(), 2);
    }

    EXPECT_TRUE(possibleWhiteMoves[0].gameState.isFree({2, 0}));
    EXPECT_FALSE(possibleWhiteMoves[0].gameState.isFree({1, 3}));
}

TEST(GameController, GameEndNoPawns)
{
    Board board{};
    GameState gameState(std::move(board));
    GameController controller(gameState);
    EXPECT_EQ(controller.gameResult(FigureColor::White), GameResult::BlackWin);
    EXPECT_EQ(controller.gameResult(FigureColor::Black), GameResult::WhiteWin);
}
TEST(GameController, GameEndWhiteNoMove)
{
    //    3---
    //    2--x
    //    1-x-
    //    0o--
    //     012
    Board board{};
    board[0][0] = FigureState{FigureType::Pawn, FigureColor::White};
    board[1][1] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[2][2] = FigureState{FigureType::Pawn, FigureColor::Black};
    GameState gameState(std::move(board));
    GameController controller(gameState);
    EXPECT_EQ(controller.gameResult(FigureColor::White), GameResult::BlackWin);
    EXPECT_EQ(controller.gameResult(FigureColor::Black), GameResult::GameOn);
}
TEST(GameController, GameEndBlackNoMove)
{
    //    3-----
    //    2--x--
    //    1-o-o-
    //    0o---o
    //     01234
    Board board{};
    board[0][0] = FigureState{FigureType::Pawn, FigureColor::White};
    board[1][1] = FigureState{FigureType::Pawn, FigureColor::White};
    board[2][2] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[1][3] = FigureState{FigureType::Pawn, FigureColor::White};
    board[0][4] = FigureState{FigureType::Pawn, FigureColor::White};
    GameState gameState(std::move(board));
    GameController controller(gameState);
    EXPECT_EQ(controller.gameResult(FigureColor::White), GameResult::GameOn);
    EXPECT_EQ(controller.gameResult(FigureColor::Black), GameResult::WhiteWin);
}
TEST(GameController, PawnNoMoves)
{
    //    7o--o
    //    2----
    //    1----
    //    0x--x
    //     0127
    Board board{};
    board[0][0] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[0][7] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[7][0] = FigureState{FigureType::Pawn, FigureColor::White};
    board[7][7] = FigureState{FigureType::Pawn, FigureColor::White};
    GameState gameState(std::move(board));
    GameController controller(gameState);
    EXPECT_EQ(controller.gameResult(FigureColor::White), GameResult::BlackWin);
    EXPECT_EQ(controller.gameResult(FigureColor::Black), GameResult::WhiteWin);
}
TEST(GameController, PawnSingleMoves)
{
    //    3----
    //    2x-o-
    //    1----
    //    0----
    //     0123
    Board board{};
    board[2][0] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[2][2] = FigureState{FigureType::Pawn, FigureColor::White};
    GameState gameState(std::move(board));
    GameController controller(gameState);
    const auto blackMoves = controller.getPossibleMoves(FigureColor::Black);
    const auto whiteMoves = controller.getPossibleMoves(FigureColor::White);
    EXPECT_EQ(blackMoves.size(), 1);
    EXPECT_EQ(whiteMoves.size(), 2);
    Move blackMove1{{2, 0}, {1, 1}};
    Move whiteMove1{{2, 2}, {3, 1}};
    Move whiteMove2{{2, 2}, {3, 3}};
    EXPECT_EQ(blackMoves.at(0).move, blackMove1);
    EXPECT_EQ(whiteMoves.at(0).move, whiteMove1);
    EXPECT_EQ(whiteMoves.at(1).move, whiteMove2);
}
TEST(GameController, PawnKingChange)
{
    //    4---
    //    3---
    //    2---
    //    1-x-
    //    0---
    //     012
    Board board{};
    board[1][1] = FigureState{FigureType::Pawn, FigureColor::Black};
    GameState gameState(std::move(board));
    GameController controller(gameState);
    const auto blackMoves = controller.getPossibleMoves(FigureColor::Black);
    EXPECT_EQ(blackMoves.size(), 2);
    EXPECT_EQ(blackMoves.at(0).gameState.pawnAtPosition({0, 0}).type, FigureType::King);
}
TEST(GameController, PawnsBeatsOnly)
{
    //    3------
    //    2--x-x-
    //    1---o--
    //    0-----
    //     012345
    Board board{};
    board[1][3] = {FigureColor::White};
    board[2][2] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[2][4] = FigureState{FigureType::Pawn, FigureColor::Black};
    GameState gameState(std::move(board));
    GameController controller(gameState);
    EXPECT_EQ(controller.gameResult(FigureColor::White), GameResult::GameOn);
    EXPECT_EQ(controller.gameResult(FigureColor::Black), GameResult::GameOn);
    const auto blackMoves = controller.getPossibleMoves(FigureColor::Black);
    const auto whiteMoves = controller.getPossibleMoves(FigureColor::White);
    EXPECT_EQ(blackMoves.size(), 2);
    EXPECT_EQ(whiteMoves.size(), 2);
    Move blackMove1{{2, 2}, {0, 4}};
    Move blackMove2{{2, 4}, {0, 2}};
    EXPECT_EQ(blackMoves.at(0).move, blackMove1);
    EXPECT_TRUE(blackMoves.at(0).gameState.isFree({1, 3}));
    EXPECT_EQ(blackMoves.at(1).move, blackMove2);
    EXPECT_TRUE(blackMoves.at(1).gameState.isFree({1, 3}));
    Move whiteMove1{{1, 3}, {3, 1}};
    Move whiteMove2{{1, 3}, {3, 5}};
    EXPECT_EQ(whiteMoves.at(0).move, whiteMove1);
    EXPECT_TRUE(whiteMoves.at(0).gameState.isFree({2, 2}));
    EXPECT_EQ(whiteMoves.at(1).move, whiteMove2);
    EXPECT_TRUE(whiteMoves.at(1).gameState.isFree({2, 4}));
}
TEST(GameController, PawnsBeatBlocked)
{
    //    3------
    //    2------
    //    1-x----
    //    0o-----
    //     012345
    Board board{};
    board[0][0] = {FigureColor::White};
    board[1][1] = FigureState{FigureType::Pawn, FigureColor::Black};
    GameState gameState(std::move(board));
    GameController controller(gameState);
    const auto blackMoves = controller.getPossibleMoves(FigureColor::Black);
    EXPECT_EQ(blackMoves.size(), 1);
    Move blackMove1{{1, 1}, {0, 2}};
    EXPECT_EQ(blackMoves.at(0).move, blackMove1);
}
TEST(GameController, PawnsMultiBeats)
{
    //    2-------
    //    1-x-x-x-
    //    0o------
    //     0123456
    Board board{};
    board[0][0] = {FigureColor::White};
    board[1][1] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[1][3] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[1][5] = FigureState{FigureType::Pawn, FigureColor::Black};
    GameState gameState(std::move(board));
    GameController controller(gameState);
    const auto whiteMoves = controller.getPossibleMoves(FigureColor::White);
    EXPECT_EQ(whiteMoves.size(), 1);
    Move whiteMove1{{0, 0}, {2, 2}, {0, 4}, {2, 6}};
    EXPECT_EQ(whiteMoves.at(0).move, whiteMove1);
    EXPECT_TRUE(whiteMoves.at(0).gameState.isFree({1, 1}));
    EXPECT_TRUE(whiteMoves.at(0).gameState.isFree({1, 3}));
    EXPECT_TRUE(whiteMoves.at(0).gameState.isFree({1, 5}));
}

TEST(GameController, PawnsMultiBeatsSelectsStronger)
{
    //    4-------
    //    3---x---
    //    2-------
    //    1-x-x-x-
    //    0o------
    //     0123456
    Board board{};
    board[0][0] = {FigureColor::White};
    board[1][1] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[1][3] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[1][5] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[3][3] = FigureState{FigureType::Pawn, FigureColor::Black};
    GameState gameState(std::move(board));
    GameController controller(gameState);
    const auto whiteMoves = controller.getPossibleMoves(FigureColor::White);
    EXPECT_EQ(whiteMoves.size(), 1);
    Move whiteMove1{{0, 0}, {2, 2}, {0, 4}, {2, 6}};
    EXPECT_EQ(whiteMoves.at(0).move, whiteMove1);
    EXPECT_TRUE(whiteMoves.at(0).gameState.isFree({1, 1}));
    EXPECT_TRUE(whiteMoves.at(0).gameState.isFree({1, 3}));
    EXPECT_TRUE(whiteMoves.at(0).gameState.isFree({1, 5}));
}
TEST(GameController, PawnsMultiBeatsMultiOptions)
{
    //    6-------
    //    5-----x-
    //    4-------
    //    3---x---
    //    2-------
    //    1-x-x-x-
    //    0o------
    //     0123456
    Board board{};
    board[0][0] = {FigureColor::White};
    board[1][1] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[1][3] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[1][5] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[3][3] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[5][5] = FigureState{FigureType::Pawn, FigureColor::Black};
    GameState gameState(std::move(board));
    GameController controller(gameState);
    const auto whiteMoves = controller.getPossibleMoves(FigureColor::White);
    EXPECT_EQ(whiteMoves.size(), 2);
    Move whiteMove1{{0, 0}, {2, 2}, {0, 4}, {2, 6}};
    Move whiteMove2{{0, 0}, {2, 2}, {4, 4}, {6, 6}};
    EXPECT_EQ(whiteMoves.at(0).move, whiteMove1);
    EXPECT_EQ(whiteMoves.at(1).move, whiteMove2);
    EXPECT_TRUE(whiteMoves.at(0).gameState.isFree({1, 1}));
    EXPECT_TRUE(whiteMoves.at(0).gameState.isFree({1, 3}));
    EXPECT_TRUE(whiteMoves.at(0).gameState.isFree({1, 5}));
    EXPECT_TRUE(whiteMoves.at(1).gameState.isFree({1, 1}));
    EXPECT_TRUE(whiteMoves.at(1).gameState.isFree({3, 3}));
    EXPECT_TRUE(whiteMoves.at(1).gameState.isFree({5, 5}));
}

TEST(GameController, KingsMoves)
{
    //    1---X
    //    0O---
    //     0123456
    Board board{};
    board[0][0] = FigureState{FigureType::King, FigureColor::White};
    board[3][1] = FigureState{FigureType::King, FigureColor::Black};
    GameState gameState(std::move(board));
    GameController controller(gameState);
    const auto whiteMoves = controller.getPossibleMoves(FigureColor::White);
    const auto blackMoves = controller.getPossibleMoves(FigureColor::Black);
    EXPECT_EQ(whiteMoves.size(), 7);
    EXPECT_EQ(blackMoves.size(), 9);
}

TEST(GameController, KingsBlockedMoves)
{
    //    4----X
    //    3---X-
    //    2-----
    //    1-----
    //    0O----
    //     0123456
    Board board{};
    board[0][0] = FigureState{FigureType::King, FigureColor::White};
    board[3][3] = FigureState{FigureType::King, FigureColor::Black};
    board[4][4] = FigureState{FigureType::King, FigureColor::Black};
    GameState gameState(std::move(board));
    GameController controller(gameState);
    const auto whiteMoves = controller.getPossibleMoves(FigureColor::White);
    EXPECT_EQ(whiteMoves.size(), 2);
}
TEST(GameController, KingsMultiBeat)
{
    //    7-X-----
    //    6--------
    //    5---X----
    //    4--------
    //    3---X-X--
    //    2--------
    //    1-X---X--
    //    0O-------
    //     01234567

    Board board{};
    board[0][0] = FigureState{FigureType::King, FigureColor::White};
    board[1][1] = FigureState{FigureType::King, FigureColor::Black};
    board[3][3] = FigureState{FigureType::King, FigureColor::Black};
    board[3][5] = FigureState{FigureType::King, FigureColor::Black};
    board[1][5] = FigureState{FigureType::King, FigureColor::Black};
    board[5][3] = FigureState{FigureType::King, FigureColor::Black};
    board[7][1] = FigureState{FigureType::King, FigureColor::Black};
    GameState gameState(std::move(board));
    GameController controller(gameState);
    const auto whiteMoves = controller.getPossibleMoves(FigureColor::White);
    EXPECT_EQ(whiteMoves.size(), 1);
    const auto whiteMove1 =
        Move{Position{0, 0}, Position{2, 2}, Position{4, 4}, Position{6, 2}, Position{2, 6}, Position{0, 4}};

    EXPECT_EQ(whiteMoves.at(0).move, whiteMove1);
}
