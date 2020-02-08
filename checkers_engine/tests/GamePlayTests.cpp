#include <gtest/gtest.h>

#include "GamePlay.hpp"

namespace
{
const auto chooseFirstMoveStrategy = [](const InitialGameState&,
                                        const std::vector<GameStateWithMove>& gameStatesWithMove) {
    return gameStatesWithMove.front();
};
}

TEST(GamePlay, ShouldThrowWhenWhiteMoveNotAllowed)
{
    GameState gameState;
    GameState invalidGameState;
    GamePlay gamePlay{gameState,
                      [invalidGameState](const InitialGameState&, const std::vector<GameStateWithMove>&) {
                          return GameStateWithMove{invalidGameState, Move{}};
                      },
                      chooseFirstMoveStrategy};
    EXPECT_THROW(gamePlay.start(), std::runtime_error);
}

TEST(GamePlay, ShouldThrowWhenBlackMoveNotAllowed)
{
    GameState gameState;
    GameState invalidGameState;
    GamePlay gamePlay{gameState,
                      chooseFirstMoveStrategy,
                      [invalidGameState](const InitialGameState&, const std::vector<GameStateWithMove>&) {
                          return GameStateWithMove{invalidGameState, Move{}};
                      }};
    EXPECT_THROW(gamePlay.start(), std::runtime_error);
}

TEST(GamePlay, ShouldEndWithBlackWinWhenWhiteHasNoMove)
{
    Board board{};
    board[1][1] = FigureState{FigureType::Pawn, FigureColor::Black};
    GameState gameState{std::move(board)};
    GamePlay gamePlay{gameState, chooseFirstMoveStrategy, chooseFirstMoveStrategy};
    EXPECT_EQ(gamePlay.start(), GameResult::BlackWin);
}

TEST(GamePlay, ShouldEndWithWhiteWinWhenBlackHasNoMove)
{
    Board board{};
    board[1][1] = FigureState{FigureType::Pawn, FigureColor::White};
    GameState gameState{std::move(board)};
    GamePlay gamePlay{gameState, chooseFirstMoveStrategy, chooseFirstMoveStrategy};
    EXPECT_EQ(gamePlay.start(), GameResult::WhiteWin);
}

TEST(GamePlay, ShouldInterchangePlayersMoves)
{
    bool whiteMove = true;
    Board board{};
    board[1][1] = FigureState{FigureType::Pawn, FigureColor::White};
    GameState gameState{std::move(board)};

    const auto chooseFirstMoveStrategyWithMoveCheckWhite =
        [&whiteMove, &gameState](
            const InitialGameState& initialGameState, const std::vector<GameStateWithMove>& gameStatesWithMove) {
            EXPECT_TRUE(whiteMove);
            EXPECT_EQ(gameState, initialGameState);
            whiteMove = false;
            return chooseFirstMoveStrategy(gameState, gameStatesWithMove);
        };
    const auto chooseFirstMoveStrategyWithMoveCheckBlack =
        [&whiteMove, &gameState](
            const InitialGameState& initialGameState, const std::vector<GameStateWithMove>& gameStatesWithMove) {
            EXPECT_FALSE(whiteMove);
            EXPECT_EQ(gameState, initialGameState);
            whiteMove = true;
            return chooseFirstMoveStrategy(gameState, gameStatesWithMove);
        };
    GamePlay gamePlay{gameState, chooseFirstMoveStrategyWithMoveCheckWhite, chooseFirstMoveStrategyWithMoveCheckBlack};
    gamePlay.start();
}

TEST(GamePlay, ShouldDrawWhenNumberOfMovesWithNoBeatsExeedes20)
{
    int numberOfMoves = 0;
    Board board{};
    board[1][1] = FigureState{FigureType::King, FigureColor::White};
    board[7][1] = FigureState{FigureType::King, FigureColor::Black};

    GameState inGameGameState{std::move(board)};
    const auto goToCornerEndlessStrategyWhite =
        [&numberOfMoves,
         &inGameGameState](const InitialGameState&, const std::vector<GameStateWithMove>& gameStatesWithMove) {
            numberOfMoves++;

            const auto startPosition = gameStatesWithMove.front().move.front();
            if (startPosition == Position{0, 0})
            {
                inGameGameState.movePawn({0, 0}, {1, 1});
            }
            else
            {
                inGameGameState.movePawn({1, 1}, {0, 0});
            }
            return GameStateWithMove{inGameGameState, Move{}};
        };
    const auto goToCornerEndlessStrategyBlack =
        [&numberOfMoves,
         &inGameGameState](const InitialGameState&, const std::vector<GameStateWithMove>& gameStatesWithMove) {
            numberOfMoves++;

            const auto startPosition = gameStatesWithMove.front().move.front();
            if (startPosition == Position{1, 7})
            {
                inGameGameState.movePawn({1, 7}, {7, 1});
            }
            else
            {
                inGameGameState.movePawn({7, 1}, {1, 7});
            }
            return GameStateWithMove{inGameGameState, Move{}};
        };

    GameState gameState = inGameGameState;
    GamePlay gamePlay{gameState, goToCornerEndlessStrategyWhite, goToCornerEndlessStrategyBlack};
    EXPECT_EQ(gamePlay.start(), GameResult::Draw);
    EXPECT_EQ(numberOfMoves, 20);
}

TEST(GamePlay, ShouldDrawWhenNumberOfMovesWithNoBeatsExeedes20WithResetCounterAfterWhitePawnBeaten)
{
    int numberOfMoves = 0;
    Board board{};
    board[1][1] = FigureState{FigureType::King, FigureColor::White};
    board[1][5] = FigureState{FigureType::King, FigureColor::White};
    board[7][1] = FigureState{FigureType::King, FigureColor::Black};

    GameState inGameGameState{std::move(board)};
    const auto goToCornerEndlessStrategyWhite =
        [&numberOfMoves,
         &inGameGameState](const InitialGameState&, const std::vector<GameStateWithMove>& gameStatesWithMove) {
            numberOfMoves++;
            if (numberOfMoves == 9)
            {
                inGameGameState.movePawn({1, 5}, {2, 6});
                return GameStateWithMove{inGameGameState, Move{}};
            }
            const auto startPosition = gameStatesWithMove.front().move.front();
            if (startPosition == Position{0, 0})
            {
                inGameGameState.movePawn({0, 0}, {1, 1});
            }
            else
            {
                inGameGameState.movePawn({1, 1}, {0, 0});
            }
            return GameStateWithMove{inGameGameState, Move{}};
        };
    const auto goToCornerEndlessStrategyBlack =
        [&numberOfMoves,
         &inGameGameState](const InitialGameState&, const std::vector<GameStateWithMove>& gameStatesWithMove) {
            numberOfMoves++;

            const auto startPosition = gameStatesWithMove.front().move.front();
            if (startPosition == Position{1, 7})
            {
                inGameGameState.movePawn({1, 7}, {7, 1});
            }
            else
            {
                inGameGameState.movePawn({7, 1}, {1, 7});
            }
            if (numberOfMoves == 10)
            {
                inGameGameState.removePawn({2, 6});
                return GameStateWithMove{inGameGameState, Move{}};
            }
            return GameStateWithMove{inGameGameState, Move{}};
        };

    GameState gameState = inGameGameState;
    GamePlay gamePlay{gameState, goToCornerEndlessStrategyWhite, goToCornerEndlessStrategyBlack};
    EXPECT_EQ(gamePlay.start(), GameResult::Draw);
    EXPECT_EQ(numberOfMoves, 30);
}

TEST(GamePlay, ShouldDrawWhenNumberOfMovesWithNoBeatsExeedes20WithResetCounterAfterBlackPawnBeaten)
{
    int numberOfMoves = 0;
    Board board{};
    board[1][1] = FigureState{FigureType::King, FigureColor::White};
    board[4][2] = FigureState{FigureType::King, FigureColor::Black};
    board[7][1] = FigureState{FigureType::King, FigureColor::Black};

    GameState inGameGameState{std::move(board)};
    const auto goToCornerEndlessStrategyWhite =
        [&numberOfMoves,
         &inGameGameState](const InitialGameState&, const std::vector<GameStateWithMove>& gameStatesWithMove) {
            numberOfMoves++;
            const auto startPosition = gameStatesWithMove.front().move.front();
            if (startPosition == Position{7, 7})
            {
                inGameGameState.movePawn({7, 7}, {1, 1});
            }
            else
            {
                inGameGameState.movePawn({1, 1}, {7, 7});
            }
            if (numberOfMoves == 11)
            {
                inGameGameState.removePawn({3, 3});
                return GameStateWithMove{inGameGameState, Move{}};
            }
            return GameStateWithMove{inGameGameState, Move{}};
        };
    const auto goToCornerEndlessStrategyBlack =
        [&numberOfMoves,
         &inGameGameState](const InitialGameState&, const std::vector<GameStateWithMove>& gameStatesWithMove) {
            numberOfMoves++;
            if (numberOfMoves == 10)
            {
                inGameGameState.movePawn({4, 2}, {3, 3});
                return GameStateWithMove{inGameGameState, Move{}};
            }
            const auto startPosition = gameStatesWithMove.front().move.front();
            if (startPosition == Position{1, 7})
            {
                inGameGameState.movePawn({1, 7}, {7, 1});
            }
            else
            {
                inGameGameState.movePawn({7, 1}, {1, 7});
            }
            return GameStateWithMove{inGameGameState, Move{}};
        };

    GameState gameState = inGameGameState;
    GamePlay gamePlay{gameState, goToCornerEndlessStrategyWhite, goToCornerEndlessStrategyBlack};
    EXPECT_EQ(gamePlay.start(), GameResult::Draw);
    EXPECT_EQ(numberOfMoves, 31);
}
