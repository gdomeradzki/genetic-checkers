#pragma once
#include "IParrarelGameplay.hpp"

#include <mutex>
#include <optional>

class ParrarelGamePlay : public IParrarelGamePlay
{
public:
    explicit ParrarelGamePlay(unsigned int maxNumberOfThreads);

    void play(BattleList, BattleFinishCallback) override;

private:
    void threadLoop(int);

    std::optional<Battle> fetchNextBattle();

    const unsigned int maxNumberOfThreads;
    BattleFinishCallback battleFinishCallback;
    BattleList battlesLeft;
    std::mutex mutex;
};
