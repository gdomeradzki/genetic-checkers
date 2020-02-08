#include "ParrarelGameplay.hpp"

#include <cassert>
#include <future>
#include <thread>
#include "GamePlay.hpp"
#include "Helpers.hpp"

ParrarelGamePlay::ParrarelGamePlay(int maxNumberOfThreads) : maxNumberOfThreads{maxNumberOfThreads}
{
    assert(maxNumberOfThreads > 0);
}

void ParrarelGamePlay::play(BattleList battleList, BattleFinishCallback callback)
{
    battlesLeft = std::move(battleList);
    battleFinishCallback = std::move(callback);
    std::vector<std::thread> threads;
    Logger::log();
    for (int i = 0; i < maxNumberOfThreads; i++)
    {
        std::thread t(&ParrarelGamePlay::threadLoop, this);
        threads.emplace_back(std::move(t));
    }
    for (int i = 0; i < maxNumberOfThreads; i++)
    {
        if (threads.at(i).joinable())
        {
            threads.at(i).join();
        }
    }
    Logger::log("");
}

void ParrarelGamePlay::threadLoop()
{
    while (true)
    {
        auto battle = fetchNextBattle();
        if (!battle)
        {
            break;
        }
        GameState gameState;
        GamePlay gameplay{gameState,
                          [&battle](const InitialGameState& initialGameState, const std::vector<GameStateWithMove>&) {
                              return battle->whitePlayerStrategy.getMove(initialGameState, FigureColor::White);
                          },
                          [&battle](const InitialGameState& initialGameState, const std::vector<GameStateWithMove>&) {
                              return battle->blackPlayerStrategy.getMove(initialGameState, FigureColor::Black);
                          }};
        const auto gameResult = gameplay.start();
        battle->result = gameResult;
        battleFinishCallback(battle.value());
    }
}

std::optional<Battle> ParrarelGamePlay::fetchNextBattle()
{
    const std::lock_guard lockGuard{mutex};
    if (battlesLeft.size() % 10 == 0)
    {
        Logger::logRepeatLine("Battles left: ", battlesLeft.size());
    }
    if (battlesLeft.empty())
    {
        return std::nullopt;
    }
    const auto lastBattle = battlesLeft.back();
    battlesLeft.pop_back();
    return lastBattle;
}
