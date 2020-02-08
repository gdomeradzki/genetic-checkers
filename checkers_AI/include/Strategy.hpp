#pragma once
#include <functional>
#include "IStrategy.hpp"

class Strategy : public IStrategy
{
public:
    GameStateWithMove getMiniMaxMove(const GameState&, EvaluationFunction, FigureColor, int maxDepth) const override;
    GameStateWithMove getRandomMove(const std::vector<GameStateWithMove>&) const override;
};
