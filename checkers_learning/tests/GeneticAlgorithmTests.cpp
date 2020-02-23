#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "GeneticAlgorithm.hpp"
#include "tests/mocks/Mocks.hpp"

class ParrarelGamePlayMock : public IParrarelGamePlay
{
public:
    MOCK_METHOD(void, play, (BattleList, BattleFinishCallback), (override));
};

class RandomEngineMock : public IRandomEngine
{
public:
    MOCK_METHOD(unsigned int, getRandomValue, (unsigned int, unsigned int), (const, override));
};
bool operator==(const Battle& first, const Battle& second)
{
    return first.result == second.result && first.blackPlayerId == second.blackPlayerId &&
        first.whitePlayerId == second.whitePlayerId && first.blackPlayerStrategy == second.blackPlayerStrategy &&
        first.whitePlayerStrategy == second.whitePlayerStrategy;
}
namespace
{
constexpr auto populationLimit = 5;
constexpr auto regenerationLimit = 3;
constexpr auto mutationsLimit = 2;
constexpr auto minimaxDeep = 6;

constexpr auto randomGeneValue = 123;
constexpr auto earlyGameLimit = 8;
constexpr auto midGameLimit = 4;

Population cratePopulation()
{
    Population population(populationLimit);
    for (int i = 0; i < populationLimit; i++)
    {
        for (int j = 0; j < totalNumberOfGenes; j++)
        {
            population.at(i).genes.at(j).factor = (i + 1) * 100 + j;
        }
    }
    return population;
}
} // namespace

using ::testing::_;
using ::testing::Invoke;
using ::testing::InvokeArgument;
using ::testing::InvokeWithoutArgs;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::StrictMock;
struct GeneticAlgorithmTests : public ::testing::Test
{
    GeneticAlgorithmTests()
        : sut{populationLimit,
              regenerationLimit,
              mutationsLimit,
              minimaxDeep,
              parrarelGamePlayMock,
              randomEngineMock,
              strategyMock,
              calculatorMock,
              [](OperationsDone, OperationsTotal) {}}
    {
    }
    void mockRandomCreation()
    {
        EXPECT_CALL(randomEngineMock, getRandomValue(Calculator::minValue, Calculator::maxValue))
            .Times(totalNumberOfGenes * populationLimit)
            .WillRepeatedly(Return(randomGeneValue));
    }
    Heuristics createHeuritics(
        MetricsWithFactors earlyGameMetrics,
        MetricsWithFactors midGameMetrics,
        MetricsWithFactors lateGameMetrics)
    {
        return Heuristics{strategyMock,
                          calculatorMock,
                          std::move(earlyGameMetrics),
                          std::move(midGameMetrics),
                          std::move(lateGameMetrics),
                          earlyGameLimit,
                          midGameLimit,
                          minimaxDeep};
    }
    BattleList prepareBattleList()
    {
        BattleList expectedBattleList;
        for (auto i = 0u; i < populationLimit; i++)
        {
            for (auto j = 0u; j < populationLimit; j++)
            {
                if (i == j) // Shouldn't play against itself
                {
                    continue;
                }
                MetricsWithFactors whiteEarlyGameMetrics;
                MetricsWithFactors whiteMidGameMetrics;
                MetricsWithFactors whiteLateGameMetrics;
                for (auto k = 0u; k < metricsNumber; k++)
                {
                    whiteEarlyGameMetrics.insert(MetricFactor{static_cast<Metric>(k), (i + 1u) * 100 + k});
                }
                for (auto k = 0u; k < metricsNumber; k++)
                {
                    whiteMidGameMetrics.insert(MetricFactor{static_cast<Metric>(k), (i + 1) * 100 + k});
                }
                for (auto k = 0u; k < metricsNumber; k++)
                {
                    whiteLateGameMetrics.insert(MetricFactor{static_cast<Metric>(k), (i + 1) * 100 + k});
                }
                MetricsWithFactors blackEarlyGameMetrics;
                MetricsWithFactors blackMidGameMetrics;
                MetricsWithFactors blackLateGameMetrics;
                for (auto k = 0u; k < metricsNumber; k++)
                {
                    blackEarlyGameMetrics.insert(MetricFactor{static_cast<Metric>(k), (j + 1) * 100 + k});
                }
                for (auto k = 0u; k < metricsNumber; k++)
                {
                    blackMidGameMetrics.insert(MetricFactor{static_cast<Metric>(k), (j + 1) * 100 + k});
                }
                for (int k = 0u; k < metricsNumber; k++)
                {
                    blackLateGameMetrics.insert(MetricFactor{static_cast<Metric>(k), (j + 1) * 100 + k});
                }

                Battle battle{createHeuritics(
                                  std::move(whiteEarlyGameMetrics),
                                  std::move(whiteMidGameMetrics),
                                  std::move(whiteLateGameMetrics)),
                              createHeuritics(
                                  std::move(blackEarlyGameMetrics),
                                  std::move(blackMidGameMetrics),
                                  std::move(blackLateGameMetrics)),
                              i,
                              j,
                              GameResult::GameOn};
                expectedBattleList.emplace_back(std::move(battle));
            }
        }
        return expectedBattleList;
    }

    StrictMock<ParrarelGamePlayMock> parrarelGamePlayMock;
    StrictMock<RandomEngineMock> randomEngineMock;
    StrictMock<StrategyMock> strategyMock;
    StrictMock<CalculatorMock> calculatorMock;

    GeneticAlgorithm sut;
};

TEST_F(GeneticAlgorithmTests, createRandomPopulation)
{
    mockRandomCreation();
    sut.createRandomPopulation();
    const auto& randomPopulation = sut.getPopulation();
    for (const auto& genotpe : randomPopulation)
    {
        for (const auto& gene : genotpe.genes)
        {
            EXPECT_EQ(gene.factor, randomGeneValue);
        }
    }
}
TEST_F(GeneticAlgorithmTests, shouldNotLoadPopulationWithWrongSize)
{
    Population population(1);
    EXPECT_THROW(sut.loadPopulation(population), std::runtime_error);
}
TEST_F(GeneticAlgorithmTests, mainLoop)
{
    sut.loadPopulation(cratePopulation());

    const auto expectedBattleList = prepareBattleList();

    Heuristics h1 = createHeuritics({}, {}, {});
    Heuristics h2 = createHeuritics({}, {}, {});
    Battle battle1{std::move(h1), std::move(h2), 1, 4, GameResult::WhiteWin};
    Battle battle2{std::move(h1), std::move(h2), 0, 1, GameResult::BlackWin};
    Battle battle3{std::move(h1), std::move(h2), 2, 3, GameResult::Draw};

    auto checkPopulationBeforeSelection = [this]() {
        const auto& population = sut.getPopulation();
        EXPECT_EQ(population.size(), populationLimit);
        EXPECT_EQ(population.at(0).fitness, 0);
        EXPECT_EQ(population.at(1).fitness, 6);
        EXPECT_EQ(population.at(2).fitness, 1);
        EXPECT_EQ(population.at(3).fitness, 1);
        EXPECT_EQ(population.at(4).fitness, 0);
    };
    EXPECT_CALL(parrarelGamePlayMock, play(expectedBattleList, _))
        .WillOnce(DoAll(
            InvokeArgument<1>(battle1),
            InvokeArgument<1>(battle2),
            InvokeArgument<1>(battle3),
            InvokeWithoutArgs(checkPopulationBeforeSelection)));

    enum class AlgorithmStatus
    {
        CrossDuplicatedWhenFirstCall,
        CrossBreeding,
        Mutation
    };
    AlgorithmStatus currentStatus{AlgorithmStatus::CrossDuplicatedWhenFirstCall};
    int stageCalls{0};
    auto randomCallsExpectations = [this, &currentStatus, &stageCalls](int from, int to) {
        switch (currentStatus)
        {
            case AlgorithmStatus::CrossDuplicatedWhenFirstCall:
            {
                EXPECT_EQ(from, 0);
                EXPECT_EQ(to, regenerationLimit - 1);
                if (stageCalls == 1)
                {
                    currentStatus = AlgorithmStatus::CrossBreeding;
                    stageCalls = 0;
                }
                else
                {
                    const auto& population = sut.getPopulation();
                    EXPECT_EQ(population.size(), regenerationLimit);
                    EXPECT_EQ(population.at(0).fitness, 6);
                    EXPECT_EQ(population.at(1).fitness, 1);
                    EXPECT_EQ(population.at(2).fitness, 1);
                    stageCalls++;
                }
                return 2;
            }
            case AlgorithmStatus::CrossBreeding:
            {
                if (stageCalls % (totalNumberOfGenes + 2) == 0)
                {
                    EXPECT_EQ(from, 0);
                    EXPECT_EQ(to, regenerationLimit - 1);
                    stageCalls++;
                    return 1;
                }
                if (stageCalls % (totalNumberOfGenes + 2) == 1)
                {
                    EXPECT_EQ(from, 0);
                    EXPECT_EQ(to, regenerationLimit - 1);
                    stageCalls++;
                    return 0;
                }
                const int expectedGeneNumber = stageCalls % (totalNumberOfGenes + 2) - 2;
                EXPECT_EQ(from, 200 + expectedGeneNumber);
                EXPECT_EQ(to, 300 + expectedGeneNumber);
                stageCalls++;
                if (stageCalls == populationLimit * (totalNumberOfGenes + 2))
                {
                    currentStatus = AlgorithmStatus::Mutation;
                    stageCalls = 0;
                }
                return 12;
            }
            case AlgorithmStatus::Mutation:
            {
                constexpr auto numberOfGenesToMutate = 8;
                constexpr auto callsPerCycle = 2 + numberOfGenesToMutate * 2;
                if (stageCalls % callsPerCycle == 0)
                {
                    EXPECT_EQ(from, 0);
                    EXPECT_EQ(to, populationLimit - 1);
                    stageCalls++;
                    return 2;
                }
                if (stageCalls % callsPerCycle == 1)
                {
                    EXPECT_EQ(from, 1);
                    EXPECT_EQ(to, totalNumberOfGenes);
                    stageCalls++;
                    return numberOfGenesToMutate;
                }
                if ((stageCalls - 2) % 2 == 0)
                {
                    EXPECT_EQ(from, 0);
                    EXPECT_EQ(to, totalNumberOfGenes - 1);
                    stageCalls++;
                    return 2;
                }
                else
                {
                    EXPECT_EQ(Calculator::minValue, from);
                    EXPECT_EQ(Calculator::maxValue, to);
                    stageCalls++;
                    return 40;
                }
            }
        }
        return 0;
    };
    EXPECT_CALL(randomEngineMock, getRandomValue(_, _)).WillRepeatedly(Invoke(randomCallsExpectations));
    sut.run(1);
}
