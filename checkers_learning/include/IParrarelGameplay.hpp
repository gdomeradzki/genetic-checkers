#pragma once

#include <functional>
#include <vector>

#include "Heuristics.hpp"

struct Battle
{
    Heuristics whitePlayerStrategy;
    Heuristics blackPlayerStrategy;
    unsigned int whitePlayerId;
    unsigned int blackPlayerId;
    GameResult result;
};

using BattleList = std::vector<Battle>;
using BattleFinishCallback = std::function<void(const Battle&)>;

class IParrarelGamePlay
{
public:
    virtual ~IParrarelGamePlay() = default;
    virtual void play(BattleList, BattleFinishCallback) = 0;
};
