#pragma once
#include <functional>
#include "GameController.hpp"

using EvaluationFunction = std::function<int(const GameState&, FigureColor)>;
class IStrategy
{
public:
    virtual ~IStrategy() = default;

    virtual GameStateWithMove getMiniMaxMove(const GameState&, EvaluationFunction, FigureColor, int maxDepth) const = 0;
    virtual GameStateWithMove getRandomMove(const std::vector<GameStateWithMove>&) const = 0;
};
