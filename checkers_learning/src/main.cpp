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
    constexpr auto populationLimit = 10u;
    constexpr auto regenerationLimit = 5u;
    constexpr auto mutatiosLimit = 2u;
    constexpr auto minimaxDeep = 2u;
    constexpr auto generationsNumber = 5u;
    const auto threadsNumber = std::thread::hardware_concurrency();
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

    auto startTime = std::chrono::high_resolution_clock::now();
    unsigned int prevProgress{0};
    ProgressCallback progressCallback = [&startTime,
                                         &prevProgress](OperationsDone actionsDone, OperationsTotal totalActions) {
        constexpr auto totalPercents = 100u;
        const auto percentDone = actionsDone * totalPercents / totalActions;
        const auto now = std::chrono::high_resolution_clock::now();
        const auto exectionTimeInSeconds = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
        if (percentDone == prevProgress)
        {
            return;
        }
        prevProgress = percentDone;
        const auto estimatedTimeInSeconds = totalPercents * exectionTimeInSeconds / percentDone;
        Logger::log(
            "Progress: ",
            percentDone,
            "% Execution time: ",
            exectionTimeInSeconds,
            "s Estimated time: ",
            estimatedTimeInSeconds,
            "s");
    };

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
                             metricCalculator,
                             std::move(progressCallback)};
    genetic.createRandomPopulation();
    genetic.run(generationsNumber);

    const auto bestGenotype = genetic.getBestGenotype();
    File::serialize(resultFile, bestGenotype);
    return 0;
}
