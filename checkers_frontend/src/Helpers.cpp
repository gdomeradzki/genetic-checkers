#include "Helpers.hpp"
#include <fstream>
#include <iostream>
Metrics File::deserialize(const std::string& filename)
{
    constexpr auto earlyGameTag = 'e';
    constexpr auto midGameTag = 'm';
    constexpr auto lateGameTag = 'l';

    std::ifstream ifstream(filename, std::ofstream::in);
    char gameStage;
    int metricNumber;
    int factor;

    MetricsWithFactors earlyGameMetrics;
    MetricsWithFactors midGameMetrics;
    MetricsWithFactors lateGameMetrics;
    while (!ifstream.eof())
    {
        ifstream >> gameStage >> metricNumber >> factor;
        std::cout << gameStage << metricNumber << factor;
        switch (gameStage)
        {
            case earlyGameTag:
                earlyGameMetrics.insert({static_cast<Metric>(metricNumber), factor});
                break;
            case midGameTag:
                midGameMetrics.insert({static_cast<Metric>(metricNumber), factor});
                break;
            case lateGameTag:
                lateGameMetrics.insert({static_cast<Metric>(metricNumber), factor});
                break;
            default:
                throw std::runtime_error("Deseralize error!");
        }
    }
    return {std::move(earlyGameMetrics), std::move(midGameMetrics), std::move(lateGameMetrics)};
}
