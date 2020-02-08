#include "GeneticAlgorithm.hpp"

#include <assert.h>
#include <random>

#include "Helpers.hpp"
#include "MetricsCalculator.hpp"
#include "ParrarelGameplay.hpp"
#include "Strategy.hpp"

GeneticAlgorithm::GeneticAlgorithm(
    int populationLimit,
    int regenerationLimit,
    int mutationsLimit,
    int minimaxDeep,
    IParrarelGamePlay& pararrelGameplay,
    IRandomEngine& randomEngine,
    IStrategy& strategy,
    IMetricsCalculator& metricCalculator)
    : populationLimit{populationLimit}
    , regenerationLimit{regenerationLimit}
    , mutationsLimit{mutationsLimit}
    , minimaxDeep{minimaxDeep}
    , pararrelGameplay{pararrelGameplay}
    , randomEngine{randomEngine}
    , strategy{strategy}
    , metricCalculator{metricCalculator}
{
    assert(populationLimit > regenerationLimit);
    assert(mutationsLimit < regenerationLimit);

    population.resize(populationLimit);
}

void GeneticAlgorithm::run(int loops)
{
    for (int i = 0; i < loops; i++)
    {
        Logger::log("Generation number: ", i + 1);
        selection();
        crossbreeding();
        mutation();
    }
}

void GeneticAlgorithm::createRandomPopulation()
{
    Population newPopulation;
    newPopulation.resize(populationLimit);

    for (int i = 0; i < populationLimit; i++)
    {
        for (auto& gene : newPopulation.at(i).genes)
        {
            gene.factor = randomEngine.getRandomValue(Calculator::minValue, Calculator::maxValue);
        }
    }

    population = std::move(newPopulation);
}

void GeneticAlgorithm::loadPopulation(Population newPopulation)
{
    if (newPopulation.size() != populationLimit)
    {
        throw std::runtime_error("Cannot load population with different size than algorithm");
    }
    population = std::move(newPopulation);
}

const Population& GeneticAlgorithm::getPopulation() const
{
    return population;
}

Genotype GeneticAlgorithm::getBestGenotype()
{
    Logger::log("Choosing best genotype...");
    selection();
    return population.front();
}

void GeneticAlgorithm::selection()
{
    freeForAll();
    std::sort(population.begin(), population.end(), [](const auto& first, const auto& second) {
        return first.fitness > second.fitness;
    });
    population.erase(population.begin() + regenerationLimit, population.end());
}

void GeneticAlgorithm::crossbreeding()
{
    Population newPopulation;
    newPopulation.reserve(populationLimit);
    while (newPopulation.size() < populationLimit)
    {
        int firstParentIdToBreed{-1};
        int secondParentIdToBreed{-1};

        while (firstParentIdToBreed == secondParentIdToBreed)
        {
            firstParentIdToBreed = randomEngine.getRandomValue(0, population.size() - 1);
            secondParentIdToBreed = randomEngine.getRandomValue(0, population.size() - 1);
        }

        auto newGenotype = crossBreed(population.at(firstParentIdToBreed), population.at(secondParentIdToBreed));
        newPopulation.push_back(newGenotype);
    }
    population = std::move(newPopulation);
}

void GeneticAlgorithm::mutation()
{
    for (int i = 0; i < mutationsLimit; i++)
    {
        const int genotypeIndex = randomEngine.getRandomValue(0, populationLimit - 1);
        auto& genotype = population[genotypeIndex];
        const int numberOfGenesToMutate = randomEngine.getRandomValue(1, totalNumberOfGenes);
        for (int j = 0; j < numberOfGenesToMutate; j++)
        {
            const int geneToMutateIndex = randomEngine.getRandomValue(0, totalNumberOfGenes - 1);
            genotype.genes[geneToMutateIndex].factor =
                randomEngine.getRandomValue(Calculator::minValue, Calculator::maxValue);
        }
    }
}

Heuristics GeneticAlgorithm::convertGenotypeToHeuristic(const Genotype& genotype) const
{
    constexpr auto oneThirdOfTotalFiguresNumber = totalPlayerFiguresNumber / 3;

    MetricsWithFactors metricsEarlyGame;
    MetricsWithFactors metricsMidGame;
    MetricsWithFactors metricsLateGame;

    for (const auto& gene : genotype.genes)
    {
        if (gene.factor == 0)
        {
            continue;
        }
        MetricFactor metricFactor{gene.metric, gene.factor};
        switch (gene.gameStage)
        {
            case GameStage::EarlyGame:
                metricsEarlyGame.insert(std::move(metricFactor));
                break;
            case GameStage::MidGame:
                metricsMidGame.insert(std::move(metricFactor));
                break;
            case GameStage::LateGame:
                metricsLateGame.insert(std::move(metricFactor));
                break;
        }
    }
    return Heuristics{strategy,
                      metricCalculator,
                      std::move(metricsEarlyGame),
                      std::move(metricsMidGame),
                      std::move(metricsLateGame),
                      oneThirdOfTotalFiguresNumber * 2,
                      oneThirdOfTotalFiguresNumber,
                      minimaxDeep};
}

void GeneticAlgorithm::freeForAll()
{
    BattleList battleList;
    const auto populationSize = population.size();
    battleList.reserve(populationSize * (populationSize - 1));
    for (int whitePlayerIterator = 0; whitePlayerIterator < populationSize; whitePlayerIterator++)
    {
        for (int blackPlayerIterator = 0; blackPlayerIterator < populationSize; blackPlayerIterator++)
        {
            if (whitePlayerIterator == blackPlayerIterator)
            {
                continue;
            }
            Heuristics whitePlayerStrategy = convertGenotypeToHeuristic(population.at(whitePlayerIterator));
            Heuristics blackPlayerStrategy = convertGenotypeToHeuristic(population.at(blackPlayerIterator));
            Battle battle{std::move(whitePlayerStrategy),
                          std::move(blackPlayerStrategy),
                          whitePlayerIterator,
                          blackPlayerIterator,
                          GameResult::GameOn};
            battleList.emplace_back(std::move(battle));
        }
    }
    pararrelGameplay.play(std::move(battleList), [this](const Battle& battle) {
        constexpr auto pointsForWin = 3;
        constexpr auto pointsForDraw = 1;
        switch (battle.result)
        {
            case GameResult::WhiteWin:
                population.at(battle.whitePlayerId).fitness += pointsForWin;
                break;
            case GameResult::BlackWin:
                population.at(battle.blackPlayerId).fitness += pointsForWin;
                break;
            case GameResult::Draw:
                population.at(battle.whitePlayerId).fitness += pointsForDraw;
                population.at(battle.blackPlayerId).fitness += pointsForDraw;
                break;
            default:
                throw std::runtime_error("Game should have an end!");
        }
    });
}

Genotype GeneticAlgorithm::crossBreed(const Genotype& first, const Genotype& second) const
{
    Genotype result;
    for (int i = 0; i < totalNumberOfGenes; i++)
    {
        const auto minmax = std::minmax(first.genes[i].factor, second.genes[i].factor);
        result.genes[i].factor = randomEngine.getRandomValue(minmax.first, minmax.second);
    }
    return result;
}