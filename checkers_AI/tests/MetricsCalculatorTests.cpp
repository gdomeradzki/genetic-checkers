#include <gtest/gtest.h>

#include "GameState.hpp"
#include "MetricsCalculator.hpp"

struct MetricsCalculatorTests : public ::testing::Test
{
    MetricsCalculatorTests() : sut{std::make_unique<MetricsCalculator>()} {}
    std::unique_ptr<IMetricsCalculator> sut;
};

TEST_F(MetricsCalculatorTests, ShouldReturnMinValueIfMetricsAreNotPresent)
{
    EXPECT_EQ(sut->evaluate({}, GameState{}, FigureColor{}), 0);
}

TEST_F(MetricsCalculatorTests, ShouldReturnMaxOrMinValueIfGameIsWonOrLost)
{
    Board board;
    board[0][0] = FigureState{FigureColor::White};
    EXPECT_EQ(
        sut->evaluate({MetricFactor{Metric::PawnsNumber, 0}}, GameState{std::move(board)}, FigureColor::White),
        Calculator::maxValue);
    EXPECT_EQ(
        sut->evaluate({MetricFactor{Metric::PawnsNumber, 0}}, GameState{std::move(board)}, FigureColor::Black),
        Calculator::minValue);
    board[0][0] = FigureState{FigureColor::Black};
    EXPECT_EQ(
        sut->evaluate({MetricFactor{Metric::PawnsNumber, 0}}, GameState{std::move(board)}, FigureColor::White),
        Calculator::minValue);
    EXPECT_EQ(
        sut->evaluate({MetricFactor{Metric::PawnsNumber, 0}}, GameState{std::move(board)}, FigureColor::Black),
        Calculator::maxValue);
}

constexpr auto metricsHalfRange = Calculator::valueRange / 2;

TEST_F(MetricsCalculatorTests, SingleMetricCalcPawnsNumberEqual)
{
    Board board;
    board[0][0] = FigureState{FigureType::Pawn, FigureColor::White};
    board[2][2] = FigureState{FigureType::Pawn, FigureColor::Black};
    GameState game{std::move(board)};
    EXPECT_EQ(sut->evaluate({{Metric::PawnsNumber, 2}}, game, FigureColor::White), metricsHalfRange * 2);
    EXPECT_EQ(sut->evaluate({{Metric::PawnsNumber, 2}}, game, FigureColor::Black), metricsHalfRange * 2);
}
TEST_F(MetricsCalculatorTests, SingleMetricCalcPawnsNumberRatio1To12)
{
    Board board;
    board[7][7] = FigureState{FigureType::Pawn, FigureColor::White};
    for (int i = 0; i < 12; i++)
    {
        board[i / 4][(i % 4 * 2) + (i / 4 % 2)] = FigureState{FigureType::Pawn, FigureColor::Black};
    }

    GameState game{std::move(board)};
    EXPECT_EQ(sut->evaluate({{Metric::PawnsNumber}}, game, FigureColor::White), 4);
    EXPECT_EQ(sut->evaluate({{Metric::PawnsNumber}}, game, FigureColor::Black), 95);
}
TEST_F(MetricsCalculatorTests, SingleMetricCalcPawnsNumberRatio12To1)
{
    Board board;
    board[7][7] = FigureState{FigureType::Pawn, FigureColor::Black};
    for (int i = 0; i < 12; i++)
    {
        board[i / 4][(i % 4 * 2) + (i / 4 % 2)] = FigureState{FigureType::Pawn, FigureColor::White};
    }
    GameState game{std::move(board)};
    EXPECT_EQ(sut->evaluate({{Metric::PawnsNumber}}, game, FigureColor::White), 95);
    EXPECT_EQ(sut->evaluate({{Metric::PawnsNumber}}, game, FigureColor::Black), 4);
}
TEST_F(MetricsCalculatorTests, SingleMetricCalcPawnsNumberRatio2To3)
{
    Board board;
    board[0][0] = FigureState{FigureType::Pawn, FigureColor::White};
    board[1][1] = FigureState{FigureType::Pawn, FigureColor::White};
    board[4][0] = FigureState{FigureType::King, FigureColor::White};
    board[6][0] = FigureState{FigureType::King, FigureColor::White};
    board[2][2] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[3][3] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[5][5] = FigureState{FigureType::Pawn, FigureColor::Black};
    GameState game{std::move(board)};
    EXPECT_EQ(sut->evaluate({{Metric::PawnsNumber}}, game, FigureColor::White), 45);
    EXPECT_EQ(sut->evaluate({{Metric::PawnsNumber}}, game, FigureColor::Black), 54);
}
TEST_F(MetricsCalculatorTests, SingleMetricCalcPawnsNumberRatio5To2)
{
    Board board;
    board[0][0] = FigureState{FigureType::Pawn, FigureColor::White};
    board[1][1] = FigureState{FigureType::Pawn, FigureColor::White};
    board[2][2] = FigureState{FigureType::Pawn, FigureColor::White};
    board[3][3] = FigureState{FigureType::Pawn, FigureColor::White};
    board[4][0] = FigureState{FigureType::King, FigureColor::White};
    board[7][7] = FigureState{FigureType::Pawn, FigureColor::White};
    board[5][5] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[6][6] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[2][0] = FigureState{FigureType::King, FigureColor::Black};
    GameState game{std::move(board)};
    EXPECT_EQ(sut->evaluate({{Metric::PawnsNumber}}, game, FigureColor::White), 58);
    EXPECT_EQ(sut->evaluate({{Metric::PawnsNumber}}, game, FigureColor::Black), 41);
}
TEST_F(MetricsCalculatorTests, SingleMetricCalcKingsRatio3To3)
{
    Board board;
    board[0][0] = FigureState{FigureType::King, FigureColor::White};
    board[1][1] = FigureState{FigureType::Pawn, FigureColor::White};
    board[2][2] = FigureState{FigureType::King, FigureColor::White};
    board[3][3] = FigureState{FigureType::Pawn, FigureColor::White};
    board[4][0] = FigureState{FigureType::King, FigureColor::White};
    board[7][7] = FigureState{FigureType::Pawn, FigureColor::White};
    board[5][5] = FigureState{FigureType::King, FigureColor::Black};
    board[6][6] = FigureState{FigureType::King, FigureColor::Black};
    board[2][0] = FigureState{FigureType::King, FigureColor::Black};
    GameState game{std::move(board)};
    EXPECT_EQ(sut->evaluate({{Metric::KingsNumber}}, game, FigureColor::White), 50);
    EXPECT_EQ(sut->evaluate({{Metric::KingsNumber}}, game, FigureColor::Black), 50);
}

TEST_F(MetricsCalculatorTests, SingleMetricSafePawnsRatio1To3)
{
    Board board;
    board[0][0] = FigureState{FigureType::Pawn, FigureColor::White};
    board[1][1] = FigureState{FigureType::Pawn, FigureColor::White};
    board[2][2] = FigureState{FigureType::Pawn, FigureColor::White};
    board[3][3] = FigureState{FigureType::Pawn, FigureColor::White};
    board[7][1] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[0][6] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[6][0] = FigureState{FigureType::Pawn, FigureColor::Black};
    GameState game{std::move(board)};
    EXPECT_EQ(sut->evaluate({{Metric::SafePawns}}, game, FigureColor::White), 40);
    EXPECT_EQ(sut->evaluate({{Metric::SafePawns}}, game, FigureColor::Black), 60);
}
TEST_F(MetricsCalculatorTests, SingleMetricSafeKingRatio3To0)
{
    Board board;
    board[0][0] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[1][1] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[2][2] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[3][3] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[7][1] = FigureState{FigureType::King, FigureColor::White};
    board[0][6] = FigureState{FigureType::King, FigureColor::White};
    board[6][0] = FigureState{FigureType::King, FigureColor::White};
    GameState game{std::move(board)};
    EXPECT_EQ(sut->evaluate({{Metric::SafeKings}}, game, FigureColor::White), 62);
    EXPECT_EQ(sut->evaluate({{Metric::SafeKings}}, game, FigureColor::Black), 37);
}

TEST_F(MetricsCalculatorTests, SingleMetricCenterPawnsRatio2To2)
{
    Board board;
    board[0][0] = FigureState{FigureType::Pawn, FigureColor::White};
    board[1][1] = FigureState{FigureType::Pawn, FigureColor::White};
    board[4][4] = FigureState{FigureType::Pawn, FigureColor::White};
    board[3][3] = FigureState{FigureType::Pawn, FigureColor::White};
    board[7][1] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[3][5] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[4][0] = FigureState{FigureType::Pawn, FigureColor::Black};
    GameState game{std::move(board)};
    EXPECT_EQ(sut->evaluate({{Metric::CenterPawns}}, game, FigureColor::White), 50);
    EXPECT_EQ(sut->evaluate({{Metric::CenterPawns}}, game, FigureColor::Black), 50);
}
TEST_F(MetricsCalculatorTests, SingleMetricCenterKingRatio1To0)
{
    Board board;
    board[0][0] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[1][1] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[2][2] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[3][3] = FigureState{FigureType::King, FigureColor::White};
    board[7][1] = FigureState{FigureType::King, FigureColor::White};
    board[0][6] = FigureState{FigureType::King, FigureColor::White};
    board[6][0] = FigureState{FigureType::King, FigureColor::White};
    GameState game{std::move(board)};
    EXPECT_EQ(sut->evaluate({{Metric::CenterKings}}, game, FigureColor::White), 54);
    EXPECT_EQ(sut->evaluate({{Metric::CenterKings}}, game, FigureColor::Black), 45);
}
TEST_F(MetricsCalculatorTests, SingleMetricMovablePawnsRatio1To2)
{
    Board board;
    board[7][1] = FigureState{FigureType::Pawn, FigureColor::White};
    board[6][2] = FigureState{FigureType::Pawn, FigureColor::White};
    board[0][0] = FigureState{FigureType::Pawn, FigureColor::White};
    board[5][5] = FigureState{FigureType::King, FigureColor::White};
    board[1][1] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[2][2] = FigureState{FigureType::Pawn, FigureColor::Black};
    GameState game{std::move(board)};
    EXPECT_EQ(sut->evaluate({{Metric::MoveablePawns}}, game, FigureColor::White), 45);
    EXPECT_EQ(sut->evaluate({{Metric::MoveablePawns}}, game, FigureColor::Black), 54);
}
TEST_F(MetricsCalculatorTests, SingleMetricMovableKingsRatio4To1)
{
    Board board;
    board[0][0] = FigureState{FigureType::King, FigureColor::White};
    board[6][2] = FigureState{FigureType::King, FigureColor::Black};
    board[2][2] = FigureState{FigureType::King, FigureColor::White};
    board[6][6] = FigureState{FigureType::King, FigureColor::Black};
    board[7][7] = FigureState{FigureType::King, FigureColor::White};
    board[0][6] = FigureState{FigureType::King, FigureColor::White};
    board[5][5] = FigureState{FigureType::King, FigureColor::White};
    GameState game{std::move(board)};
    EXPECT_EQ(sut->evaluate({{Metric::MoveableKings}}, game, FigureColor::White), 62);
    EXPECT_EQ(sut->evaluate({{Metric::MoveableKings}}, game, FigureColor::Black), 37);
}
TEST_F(MetricsCalculatorTests, SingleMetricAggregatedDistanceToPromotionLine1To1)
{
    Board board;
    board[6][0] = FigureState{FigureType::Pawn, FigureColor::White};
    board[7][5] = FigureState{FigureType::Pawn, FigureColor::Black};
    GameState game{std::move(board)};
    EXPECT_EQ(sut->evaluate({{Metric::AggregatedDistanceToPromotionLine}}, game, FigureColor::White), 100);
    EXPECT_EQ(sut->evaluate({{Metric::AggregatedDistanceToPromotionLine}}, game, FigureColor::Black), 0);
}
TEST_F(MetricsCalculatorTests, SingleMetricAggregatedDistanceToPromotionLine6to3)
{
    Board board;
    board[0][0] = FigureState{FigureType::Pawn, FigureColor::White};
    board[2][0] = FigureState{FigureType::Pawn, FigureColor::White};
    board[4][0] = FigureState{FigureType::Pawn, FigureColor::White};
    board[6][0] = FigureState{FigureType::Pawn, FigureColor::White};
    board[6][2] = FigureState{FigureType::Pawn, FigureColor::White};
    board[5][5] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[7][5] = FigureState{FigureType::Pawn, FigureColor::Black};
    GameState game{std::move(board)};
    EXPECT_EQ(sut->evaluate({{Metric::AggregatedDistanceToPromotionLine}}, game, FigureColor::White), 62);
    EXPECT_EQ(sut->evaluate({{Metric::AggregatedDistanceToPromotionLine}}, game, FigureColor::Black), 37);
}
TEST_F(MetricsCalculatorTests, SingleMetricUnoccupiedFields3To1)
{
    Board board;
    board[0][0] = FigureState{FigureType::Pawn, FigureColor::White};
    board[0][2] = FigureState{FigureType::Pawn, FigureColor::White};
    board[0][6] = FigureState{FigureType::Pawn, FigureColor::White};
    board[6][0] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[6][2] = FigureState{FigureType::Pawn, FigureColor::White};
    board[5][5] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[7][5] = FigureState{FigureType::Pawn, FigureColor::Black};
    GameState game{std::move(board)};
    EXPECT_EQ(sut->evaluate({{Metric::NumberOfUnoccupiedFieldOnPromotionLine}}, game, FigureColor::White), 75);
    EXPECT_EQ(sut->evaluate({{Metric::NumberOfUnoccupiedFieldOnPromotionLine}}, game, FigureColor::Black), 25);
}
TEST_F(MetricsCalculatorTests, SingleMetricDefendingFigures2To3)
{
    Board board;
    board[0][0] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[0][2] = FigureState{FigureType::Pawn, FigureColor::White};
    board[2][6] = FigureState{FigureType::Pawn, FigureColor::White};
    board[6][0] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[6][2] = FigureState{FigureType::Pawn, FigureColor::White};
    board[5][5] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[7][5] = FigureState{FigureType::Pawn, FigureColor::Black};
    GameState game{std::move(board)};
    EXPECT_EQ(sut->evaluate({{Metric::DefenderFigures}}, game, FigureColor::White), 45);
    EXPECT_EQ(sut->evaluate({{Metric::DefenderFigures}}, game, FigureColor::Black), 54);
}
TEST_F(MetricsCalculatorTests, SingleMetricAttackingFigures1To3)
{
    Board board;
    board[0][0] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[1][3] = FigureState{FigureType::King, FigureColor::Black};
    board[2][4] = FigureState{FigureType::King, FigureColor::Black};
    board[0][2] = FigureState{FigureType::Pawn, FigureColor::White};
    board[2][6] = FigureState{FigureType::Pawn, FigureColor::White};
    board[6][0] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[6][2] = FigureState{FigureType::Pawn, FigureColor::White};
    board[5][5] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[7][5] = FigureState{FigureType::Pawn, FigureColor::Black};
    GameState game{std::move(board)};
    EXPECT_EQ(sut->evaluate({{Metric::AttackingFigures}}, game, FigureColor::White), 41);
    EXPECT_EQ(sut->evaluate({{Metric::AttackingFigures}}, game, FigureColor::Black), 58);
}
TEST_F(MetricsCalculatorTests, SingleMetricPawnsOnDiagonal3To1)
{
    Board board;
    board[0][0] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[1][1] = FigureState{FigureType::King, FigureColor::Black};
    board[2][2] = FigureState{FigureType::King, FigureColor::Black};
    board[3][3] = FigureState{FigureType::Pawn, FigureColor::White};
    board[4][4] = FigureState{FigureType::Pawn, FigureColor::White};
    board[5][5] = FigureState{FigureType::King, FigureColor::Black};
    board[6][6] = FigureState{FigureType::Pawn, FigureColor::White};
    board[1][5] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[7][5] = FigureState{FigureType::Pawn, FigureColor::Black};
    GameState game{std::move(board)};
    EXPECT_EQ(sut->evaluate({{Metric::PawnsOnDiagonal}}, game, FigureColor::White), 64);
    EXPECT_EQ(sut->evaluate({{Metric::PawnsOnDiagonal}}, game, FigureColor::Black), 35);
}
TEST_F(MetricsCalculatorTests, SingleMetricKingsOnDiagonal0To3)
{
    Board board;
    board[0][0] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[1][1] = FigureState{FigureType::King, FigureColor::Black};
    board[2][2] = FigureState{FigureType::King, FigureColor::Black};
    board[3][3] = FigureState{FigureType::Pawn, FigureColor::White};
    board[4][4] = FigureState{FigureType::Pawn, FigureColor::White};
    board[5][5] = FigureState{FigureType::King, FigureColor::Black};
    board[6][6] = FigureState{FigureType::Pawn, FigureColor::White};
    board[1][5] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[7][5] = FigureState{FigureType::Pawn, FigureColor::Black};
    GameState game{std::move(board)};
    EXPECT_EQ(sut->evaluate({{Metric::KingsOnDiagonal}}, game, FigureColor::White), 31);
    EXPECT_EQ(sut->evaluate({{Metric::KingsOnDiagonal}}, game, FigureColor::Black), 68);
}
TEST_F(MetricsCalculatorTests, SingleMetricPawnsOnDoubleDiagonal5To1)
{
    Board board;
    board[0][2] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[2][0] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[4][2] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[5][7] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[6][4] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[4][6] = FigureState{FigureType::Pawn, FigureColor::White};
    GameState game{std::move(board)};
    EXPECT_EQ(sut->evaluate({{Metric::PawnsOnDoubleDiagonal}}, game, FigureColor::White), 31);
    EXPECT_EQ(sut->evaluate({{Metric::PawnsOnDoubleDiagonal}}, game, FigureColor::Black), 68);
}
TEST_F(MetricsCalculatorTests, SingleMetricKingsOnDoubleDiagonal5To1)
{
    Board board;
    board[0][2] = FigureState{FigureType::King, FigureColor::Black};
    board[2][0] = FigureState{FigureType::King, FigureColor::Black};
    board[4][2] = FigureState{FigureType::King, FigureColor::Black};
    board[5][7] = FigureState{FigureType::King, FigureColor::Black};
    board[6][4] = FigureState{FigureType::King, FigureColor::Black};
    board[4][6] = FigureState{FigureType::King, FigureColor::White};
    GameState game{std::move(board)};
    EXPECT_EQ(sut->evaluate({{Metric::KingsOnDoubleDiagonal}}, game, FigureColor::White), 33);
    EXPECT_EQ(sut->evaluate({{Metric::KingsOnDoubleDiagonal}}, game, FigureColor::Black), 66);
}
TEST_F(MetricsCalculatorTests, SingleMetricTrianglePattern)
{
    Board board;
    board[5][5] = FigureState{FigureType::King, FigureColor::White};
    board[4][4] = FigureState{FigureType::King, FigureColor::Black};
    auto copyBoard = board;
    GameState game{std::move(copyBoard)};

    EXPECT_EQ(sut->evaluate({{Metric::Triangle}}, game, FigureColor::White), 50);
    EXPECT_EQ(sut->evaluate({{Metric::Triangle}}, game, FigureColor::Black), 50);

    board[0][4] = FigureState{FigureType::King, FigureColor::White};
    board[0][6] = FigureState{FigureType::Pawn, FigureColor::White};
    board[1][5] = FigureState{FigureType::King, FigureColor::White};
    board[7][1] = FigureState{FigureType::King, FigureColor::Black};
    board[7][3] = FigureState{FigureType::King, FigureColor::Black};
    board[6][2] = FigureState{FigureType::Pawn, FigureColor::Black};

    copyBoard = board;
    game = GameState{std::move(copyBoard)};
    EXPECT_EQ(sut->evaluate({{Metric::Triangle}}, game, FigureColor::White), 50);
    EXPECT_EQ(sut->evaluate({{Metric::Triangle}}, game, FigureColor::Black), 50);

    board[6][2] = FigureState{FigureType::King, FigureColor::White};
    copyBoard = board;
    game = GameState{std::move(copyBoard)};
    EXPECT_EQ(sut->evaluate({{Metric::Triangle}}, game, FigureColor::White), 100);
    EXPECT_EQ(sut->evaluate({{Metric::Triangle}}, game, FigureColor::Black), 0);

    board[0][4] = FigureState{FigureType::King, FigureColor::Black};
    copyBoard = board;
    game = GameState{std::move(copyBoard)};
    EXPECT_EQ(sut->evaluate({{Metric::Triangle}}, game, FigureColor::White), 50);
    EXPECT_EQ(sut->evaluate({{Metric::Triangle}}, game, FigureColor::Black), 50);
}

TEST_F(MetricsCalculatorTests, SingleMetricOreoPattern)
{
    Board board;
    board[5][5] = FigureState{FigureType::King, FigureColor::White};
    board[4][4] = FigureState{FigureType::King, FigureColor::Black};
    auto copyBoard = board;
    GameState game{std::move(copyBoard)};
    EXPECT_EQ(sut->evaluate({{Metric::Oreo}}, game, FigureColor::White), 50);
    EXPECT_EQ(sut->evaluate({{Metric::Oreo}}, game, FigureColor::Black), 50);

    board[0][2] = FigureState{FigureType::King, FigureColor::White};
    board[0][4] = FigureState{FigureType::Pawn, FigureColor::White};
    board[1][3] = FigureState{FigureType::King, FigureColor::White};
    board[7][3] = FigureState{FigureType::King, FigureColor::Black};
    board[7][5] = FigureState{FigureType::King, FigureColor::Black};
    board[6][4] = FigureState{FigureType::Pawn, FigureColor::Black};

    copyBoard = board;
    game = GameState{std::move(copyBoard)};
    EXPECT_EQ(sut->evaluate({{Metric::Oreo}}, game, FigureColor::White), 50);
    EXPECT_EQ(sut->evaluate({{Metric::Oreo}}, game, FigureColor::Black), 50);

    board[7][5] = FigureState{FigureType::King, FigureColor::White};
    copyBoard = board;
    game = GameState{std::move(copyBoard)};
    EXPECT_EQ(sut->evaluate({{Metric::Oreo}}, game, FigureColor::White), 100);
    EXPECT_EQ(sut->evaluate({{Metric::Oreo}}, game, FigureColor::Black), 0);

    board[0][4] = FigureState{FigureType::King, FigureColor::Black};
    copyBoard = board;
    game = GameState{std::move(copyBoard)};
    EXPECT_EQ(sut->evaluate({{Metric::Oreo}}, game, FigureColor::White), 50);
    EXPECT_EQ(sut->evaluate({{Metric::Oreo}}, game, FigureColor::Black), 50);
}

TEST_F(MetricsCalculatorTests, SingleMetricBridgePattern)
{
    Board board;
    board[5][5] = FigureState{FigureType::King, FigureColor::White};
    board[4][4] = FigureState{FigureType::King, FigureColor::Black};
    auto copyBoard = board;
    GameState game{std::move(copyBoard)};
    EXPECT_EQ(sut->evaluate({{Metric::Bridge}}, game, FigureColor::White), 50);
    EXPECT_EQ(sut->evaluate({{Metric::Bridge}}, game, FigureColor::Black), 50);

    board[0][2] = FigureState{FigureType::King, FigureColor::White};
    board[0][6] = FigureState{FigureType::Pawn, FigureColor::White};
    board[7][1] = FigureState{FigureType::King, FigureColor::Black};
    board[7][5] = FigureState{FigureType::King, FigureColor::Black};
    copyBoard = board;
    game = GameState{std::move(copyBoard)};
    EXPECT_EQ(sut->evaluate({{Metric::Bridge}}, game, FigureColor::White), 50);
    EXPECT_EQ(sut->evaluate({{Metric::Bridge}}, game, FigureColor::Black), 50);

    board[7][5] = FigureState{FigureType::King, FigureColor::White};
    copyBoard = board;
    game = GameState{std::move(copyBoard)};
    EXPECT_EQ(sut->evaluate({{Metric::Bridge}}, game, FigureColor::White), 100);
    EXPECT_EQ(sut->evaluate({{Metric::Bridge}}, game, FigureColor::Black), 0);

    board[0][6] = FigureState{FigureType::King, FigureColor::Black};
    copyBoard = board;
    game = GameState{std::move(copyBoard)};
    EXPECT_EQ(sut->evaluate({{Metric::Bridge}}, game, FigureColor::White), 50);
    EXPECT_EQ(sut->evaluate({{Metric::Bridge}}, game, FigureColor::Black), 50);
}

TEST_F(MetricsCalculatorTests, SingleMetricPawnCornerPattern)
{
    Board board;
    board[5][5] = FigureState{FigureType::King, FigureColor::White};
    board[4][4] = FigureState{FigureType::King, FigureColor::Black};
    auto copyBoard = board;
    GameState game{std::move(copyBoard)};
    EXPECT_EQ(sut->evaluate({{Metric::Corner}}, game, FigureColor::White), 50);
    EXPECT_EQ(sut->evaluate({{Metric::Corner}}, game, FigureColor::Black), 50);

    board[0][0] = FigureState{FigureType::Pawn, FigureColor::White};
    board[7][7] = FigureState{FigureType::Pawn, FigureColor::Black};
    copyBoard = board;
    game = GameState{std::move(copyBoard)};
    EXPECT_EQ(sut->evaluate({{Metric::Corner}}, game, FigureColor::White), 50);
    EXPECT_EQ(sut->evaluate({{Metric::Corner}}, game, FigureColor::Black), 50);

    board[7][7] = FigureState{FigureType::King, FigureColor::Black};
    copyBoard = board;
    game = GameState{std::move(copyBoard)};
    EXPECT_EQ(sut->evaluate({{Metric::Corner}}, game, FigureColor::White), 100);
    EXPECT_EQ(sut->evaluate({{Metric::Corner}}, game, FigureColor::Black), 0);

    board[0][0] = FigureState{FigureType::King, FigureColor::White};
    copyBoard = board;
    game = GameState{std::move(copyBoard)};
    EXPECT_EQ(sut->evaluate({{Metric::Corner}}, game, FigureColor::White), 50);
    EXPECT_EQ(sut->evaluate({{Metric::Corner}}, game, FigureColor::Black), 50);
}

TEST_F(MetricsCalculatorTests, SingleMetricKingCornerPattern)
{
    Board board;
    board[5][5] = FigureState{FigureType::King, FigureColor::White};
    board[4][4] = FigureState{FigureType::King, FigureColor::Black};
    auto copyBoard = board;
    GameState game{std::move(copyBoard)};
    EXPECT_EQ(sut->evaluate({{Metric::KingCorner}}, game, FigureColor::White), 50);
    EXPECT_EQ(sut->evaluate({{Metric::KingCorner}}, game, FigureColor::Black), 50);

    board[0][0] = FigureState{FigureType::King, FigureColor::Black};
    board[7][7] = FigureState{FigureType::King, FigureColor::White};
    copyBoard = board;
    game = GameState{std::move(copyBoard)};
    EXPECT_EQ(sut->evaluate({{Metric::KingCorner}}, game, FigureColor::White), 50);
    EXPECT_EQ(sut->evaluate({{Metric::KingCorner}}, game, FigureColor::Black), 50);

    board[7][7] = FigureState{FigureType::Pawn, FigureColor::White};
    copyBoard = board;
    game = GameState{std::move(copyBoard)};
    EXPECT_EQ(sut->evaluate({{Metric::KingCorner}}, game, FigureColor::White), 0);
    EXPECT_EQ(sut->evaluate({{Metric::KingCorner}}, game, FigureColor::Black), 100);

    board[0][0] = FigureState{FigureType::Pawn, FigureColor::Black};
    copyBoard = board;
    game = GameState{std::move(copyBoard)};
    EXPECT_EQ(sut->evaluate({{Metric::KingCorner}}, game, FigureColor::White), 50);
    EXPECT_EQ(sut->evaluate({{Metric::KingCorner}}, game, FigureColor::Black), 50);
}

TEST_F(MetricsCalculatorTests, SingleMetricDogPattern)
{
    Board board;
    board[5][5] = FigureState{FigureType::King, FigureColor::White};
    board[4][4] = FigureState{FigureType::King, FigureColor::Black};
    auto copyBoard = board;
    GameState game{std::move(copyBoard)};
    EXPECT_EQ(sut->evaluate({{Metric::KingCorner}}, game, FigureColor::White), 50);
    EXPECT_EQ(sut->evaluate({{Metric::KingCorner}}, game, FigureColor::Black), 50);

    board[0][6] = FigureState{FigureType::Pawn, FigureColor::White};
    board[1][7] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[7][1] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[6][0] = FigureState{FigureType::Pawn, FigureColor::White};
    copyBoard = board;
    game = GameState{std::move(copyBoard)};
    EXPECT_EQ(sut->evaluate({{Metric::Dog}}, game, FigureColor::White), 50);
    EXPECT_EQ(sut->evaluate({{Metric::Dog}}, game, FigureColor::Black), 50);

    board[0][6] = FigureState{FigureType::Pawn, FigureColor::Black};
    copyBoard = board;
    game = GameState{std::move(copyBoard)};
    EXPECT_EQ(sut->evaluate({{Metric::Dog}}, game, FigureColor::White), 0);
    EXPECT_EQ(sut->evaluate({{Metric::Dog}}, game, FigureColor::Black), 100);

    board[7][1] = FigureState{FigureType::King, FigureColor::Black};
    copyBoard = board;
    game = GameState{std::move(copyBoard)};
    EXPECT_EQ(sut->evaluate({{Metric::Dog}}, game, FigureColor::White), 50);
    EXPECT_EQ(sut->evaluate({{Metric::Dog}}, game, FigureColor::Black), 50);
}
TEST_F(MetricsCalculatorTests, MultiMetrics)
{
    Board board;
    board[4][0] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[1][3] = FigureState{FigureType::King, FigureColor::Black};
    board[2][4] = FigureState{FigureType::King, FigureColor::Black};
    board[0][2] = FigureState{FigureType::Pawn, FigureColor::White};
    board[2][6] = FigureState{FigureType::Pawn, FigureColor::White};
    board[6][0] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[6][2] = FigureState{FigureType::Pawn, FigureColor::White};
    board[5][5] = FigureState{FigureType::Pawn, FigureColor::Black};
    board[7][5] = FigureState{FigureType::Pawn, FigureColor::Black};
    GameState game{std::move(board)};
    const auto metrics = std::set<MetricFactor>{
        {Metric::PawnsNumber, 20}, {Metric::KingsNumber, 50}, {Metric::SafePawns, 5}, {Metric::SafeKings, 11}};
    EXPECT_EQ(sut->evaluate(metrics, game, FigureColor::White), 925);
    EXPECT_EQ(sut->evaluate(metrics, game, FigureColor::Black), 1207);
}
