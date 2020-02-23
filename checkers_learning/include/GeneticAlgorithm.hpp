#pragma once

#include <array>
#include <memory>
#include <mutex>

#include "Heuristics.hpp"
#include "IParrarelGameplay.hpp"
#include "IRandomEngine.hpp"
#include "IStrategy.hpp"
#include "Types.hpp"

using Population = std::vector<Genotype>;

class GeneticAlgorithm
{
public:
    GeneticAlgorithm(
        unsigned int populationLimit,
        unsigned int regenerationLimit,
        unsigned int mutationsLimit,
        unsigned int minimaxDeep,
        IParrarelGamePlay& pararrelGameplay,
        IRandomEngine& randomEngine,
        IStrategy& strategy,
        IMetricsCalculator& metricCalculator,
        ProgressCallback progressCallback);

    void run(int loops);

    void createRandomPopulation();
    void loadPopulation(Population);
    const Population& getPopulation() const;

    Genotype getBestGenotype();

private:
    void selection();
    void crossbreeding();
    void mutation();

    Heuristics convertGenotypeToHeuristic(const Genotype&) const;
    void freeForAll();
    Genotype crossBreed(const Genotype&, const Genotype&) const;

    void logProgressFromOneGenetation(OperationsDone, OperationsTotal) const;

    unsigned int currentGeneration{0};
    unsigned int totalGenerations{0};
    Population population;
    std::mutex finishCallbackMutex;

    const unsigned int populationLimit;
    const unsigned int regenerationLimit;
    const unsigned int mutationsLimit;
    const unsigned int minimaxDeep;

    IParrarelGamePlay& pararrelGameplay;
    IRandomEngine& randomEngine;
    IStrategy& strategy;
    IMetricsCalculator& metricCalculator;
    ProgressCallback progressCallback;
};
