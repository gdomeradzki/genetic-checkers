#include "Helpers.hpp"
#include <fstream>

std::string Logger::currentTimeFormatted()
{
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 80, "%H:%M:%S", timeinfo);
    return buffer;
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
