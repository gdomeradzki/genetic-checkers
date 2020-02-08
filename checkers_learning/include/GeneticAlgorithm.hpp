#pragma once

#include <array>
#include <memory>
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
        int populationLimit,
        int regenerationLimit,
        int mutationsLimit,
        int minimaxDeep,
        IParrarelGamePlay& pararrelGameplay,
        IRandomEngine& randomEngine,
        IStrategy& strategy,
        IMetricsCalculator& metricCalculator);

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

    Population population;

    const int populationLimit;
    const int regenerationLimit;
    const int mutationsLimit;
    const int minimaxDeep;

    IParrarelGamePlay& pararrelGameplay;
    IRandomEngine& randomEngine;
    IStrategy& strategy;
    IMetricsCalculator& metricCalculator;
};
