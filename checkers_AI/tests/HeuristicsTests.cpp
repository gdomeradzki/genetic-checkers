
#include <gtest/gtest.h>
#include "Heuristics.hpp"
#include "tests/mocks/Mocks.hpp"

constexpr auto earlyGameFiguresLimit = 7;
constexpr auto midGameFiguresLimit = 5;
constexpr auto minimaxDeep = 3;

using ::testing::_;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::StrictMock;

struct HeuristicsTests : public ::testing::Test
{
    HeuristicsTests()
        : sut{strategyMock,
              calculatorMock,
              metricsEarlyGame,
              metricsMidGame,
              metricsLateGame,
              earlyGameFiguresLimit,
              midGameFiguresLimit,
              minimaxDeep}
    {
    }
    virtual ~HeuristicsTests() {}
    StrictMock<StrategyMock> strategyMock;
    StrictMock<CalculatorMock> calculatorMock;
    MetricsWithFactors metricsEarlyGame;
    MetricsWithFactors metricsMidGame;
    MetricsWithFactors metricsLateGame;
    Heuristics sut;
};

TEST_F(HeuristicsTests, ShouldUseAppriopriateMetricWithFactorsDependingOnGameProgress)
{
    Board boardEarlyGame;
    boardEarlyGame[0][0] = FigureState{FigureColor::Black};
    boardEarlyGame[0][2] = FigureState{FigureColor::White};
    boardEarlyGame[0][4] = FigureState{FigureColor::Black};
    boardEarlyGame[0][6] = FigureState{FigureColor::White};
    boardEarlyGame[1][1] = FigureState{FigureColor::Black};
    boardEarlyGame[1][3] = FigureState{FigureColor::White};
    boardEarlyGame[1][5] = FigureState{FigureColor::Black};
    GameState earlyGameGameState{std::move(boardEarlyGame)};

    EvaluationFunction evalFunction;

    EXPECT_CALL(strategyMock, getMiniMaxMove(earlyGameGameState, _, FigureColor::White, minimaxDeep))
        .WillOnce(DoAll(SaveArg<1>(&evalFunction), Return(GameStateWithMove{})));
    sut.getMove(earlyGameGameState, FigureColor::White);
    EXPECT_CALL(calculatorMock, evaluate(metricsEarlyGame, earlyGameGameState, FigureColor::White));
    evalFunction(earlyGameGameState, FigureColor::White);

    Board boardMidGame;
    boardMidGame[0][0] = FigureState{FigureColor::Black};
    boardMidGame[0][2] = FigureState{FigureColor::White};
    boardMidGame[0][4] = FigureState{FigureColor::Black};
    boardMidGame[0][6] = FigureState{FigureColor::White};
    boardMidGame[1][1] = FigureState{FigureColor::Black};
    GameState midGameGameState{std::move(boardMidGame)};

    EXPECT_CALL(strategyMock, getMiniMaxMove(midGameGameState, _, FigureColor::White, minimaxDeep))
        .WillOnce(DoAll(SaveArg<1>(&evalFunction), Return(GameStateWithMove{})));
    sut.getMove(midGameGameState, FigureColor::White);
    EXPECT_CALL(calculatorMock, evaluate(metricsMidGame, midGameGameState, FigureColor::White));
    evalFunction(midGameGameState, FigureColor::White);

    Board lateEarlyGame;
    boardEarlyGame[0][0] = FigureState{FigureColor::Black};
    boardEarlyGame[0][2] = FigureState{FigureColor::White};
    boardEarlyGame[0][4] = FigureState{FigureColor::Black};
    GameState lateGameGameState{std::move(lateEarlyGame)};

    EXPECT_CALL(strategyMock, getMiniMaxMove(lateGameGameState, _, FigureColor::White, minimaxDeep))
        .WillOnce(DoAll(SaveArg<1>(&evalFunction), Return(GameStateWithMove{})));
    EXPECT_CALL(calculatorMock, evaluate(metricsLateGame, lateGameGameState, FigureColor::White));
    sut.getMove(lateGameGameState, FigureColor::White);
    evalFunction(lateGameGameState, FigureColor::White);
}
