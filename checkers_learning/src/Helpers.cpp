#include "Helpers.hpp"
#include <fstream>

std::string Logger::currentTimeFormatted()
{
    time_t rawtime;
    struct tm* timeinfo;
    constexpr auto bufferSize = 80u;
    std::array<char, bufferSize> buffer{};

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer.data(), bufferSize, "%H:%M:%S", timeinfo);
    return std::string(buffer.data());
}

void File::serialize(const std::string& filename, const Genotype& genotype)
{
    constexpr auto earlyGameTag = 'e';
    constexpr auto midGameTag = 'm';
    constexpr auto lateGameTag = 'l';
    std::ofstream ofstream(filename, std::ofstream::out | std::ofstream::trunc);

    for (const auto& gene : genotype.genes)
    {
        char gameStage;
        int metricNumber = static_cast<int>(gene.metric);

        switch (gene.gameStage)
        {
            case GameStage::EarlyGame:
                gameStage = earlyGameTag;
                break;
            case GameStage::MidGame:
                gameStage = midGameTag;
                break;
            case GameStage::LateGame:
                gameStage = lateGameTag;
                break;
        }
        ofstream << gameStage << " " << metricNumber << " " << gene.factor << std::endl;
    }
    ofstream.close();

    Logger::log("Best genotype saved: ", filename);
}

std::function<void(OperationsDone, OperationsTotal)> Logger::getProgressLogCallback(
    const std::chrono::high_resolution_clock::time_point& startTime)
{
    return [&startTime](OperationsDone actionsDone, OperationsTotal totalActions) {
        if (actionsDone == 0) return;
        const auto now = std::chrono::high_resolution_clock::now();
        const auto executionTimeInSeconds = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();

        const auto estimatedTimeLeftInSeconds =
            totalActions * executionTimeInSeconds / actionsDone - executionTimeInSeconds;
        constexpr auto timeUnits = 60u;
        std::string timeUnit = "sec";
        unsigned int formattedTime = estimatedTimeLeftInSeconds;
        if (estimatedTimeLeftInSeconds / (timeUnits * timeUnits) > 0)
        {
            timeUnit = "h";
            formattedTime = estimatedTimeLeftInSeconds / (timeUnits * timeUnits);
        }
        else if (estimatedTimeLeftInSeconds / timeUnits > 0)
        {
            timeUnit = "min";
            formattedTime = estimatedTimeLeftInSeconds / timeUnits;
        }
        Logger::log(
            "Execution time: ",
            executionTimeInSeconds,
            "s\tEstimated time left: ",
            formattedTime,
            timeUnit,
            "\t\tActions:",
            actionsDone,
            "/",
            totalActions);
    };
}
