#pragma once

#include <functional>
#include <memory>
#include <vector>
#include "Heuristics.hpp"

struct Battle
{
    Heuristics whitePlayerStrategy;
    Heuristics blackPlayerStrategy;
    unsigned int whitePlayerId{0};
    unsigned int blackPlayerId{0};
    GameResult result{GameResult::GameOn};
};

using BattleList = std::vector<Battle>;
using BattleFinishCallback = std::function<void(const Battle&)>;

class IParrarelGamePlay
{
public:
    IParrarelGamePlay() = default;
    IParrarelGamePlay(const IParrarelGamePlay&) = delete;
    IParrarelGamePlay(IParrarelGamePlay&&) = default;
    virtual ~IParrarelGamePlay() = default;

    IParrarelGamePlay& operator=(const IParrarelGamePlay&) = delete;
    IParrarelGamePlay& operator=(IParrarelGamePlay&&) = default;

    virtual void play(BattleList, BattleFinishCallback) = 0;
};
