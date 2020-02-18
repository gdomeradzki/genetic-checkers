#include <thread>
#include "GeneticAlgorithm.hpp"

#include "MetricsCalculator.hpp"
#include "ParrarelGameplay.hpp"
#include "RandomEngine.hpp"
#include "Strategy.hpp"

#include "Helpers.hpp"
using namespace std;

int main(int, char*[])
{
    auto populationLimit = 10u;
    auto regenerationLimit = 5u;
    auto mutatiosLimit = 2u;
    auto minimaxDeep = 2u;
    auto generationsNumber = 5u;
    int threadsNumber = std::thread::hardware_concurrency();
    const std::string resultFile = "bestGenotype.txt";

    Logger::log("Using:");
    Logger::log("\tPopulation Limit: ", populationLimit);
    Logger::log("\tRegenerationLimit Limit: ", regenerationLimit);
    Logger::log("\tMutatiosLimit Limit: ", mutatiosLimit);
    Logger::log("\tMinimaxDeep Limit: ", minimaxDeep);
    Logger::log("\tGenerationsNumber Limit: ", generationsNumber);
    Logger::log("\tThreads: ", threadsNumber);
    Logger::log();
    MetricsCalculator metricCalculator;
    ParrarelGamePlay parrarelGameplay{threadsNumber};
    Strategy strategy;
    std::random_device rd;
    std::mt19937 gen{rd()};
    RandomEngine randomEngine(gen);

    GeneticAlgorithm genetic{populationLimit,
                             regenerationLimit,
                             mutatiosLimit,
                             minimaxDeep,
                             parrarelGameplay,
                             randomEngine,
                             strategy,
                             metricCalculator};
    genetic.createRandomPopulation();
    genetic.run(generationsNumber);

    const auto bestGenotype = genetic.getBestGenotype();
    File::serialize(resultFile, bestGenotype);
    return 0;
}
