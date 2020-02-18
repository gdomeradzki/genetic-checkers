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
        unsigned int populationLimit,
        unsigned int regenerationLimit,
        unsigned int mutationsLimit,
        unsigned int minimaxDeep,
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

    const unsigned int populationLimit;
    const unsigned int regenerationLimit;
    const unsigned int mutationsLimit;
    const unsigned int minimaxDeep;

    IParrarelGamePlay& pararrelGameplay;
    IRandomEngine& randomEngine;
    IStrategy& strategy;
    IMetricsCalculator& metricCalculator;
};
