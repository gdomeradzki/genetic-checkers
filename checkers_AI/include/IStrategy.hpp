#pragma once
#include <functional>
#include "GameController.hpp"

using EvaluationFunction = std::function<int(const GameState&, FigureColor)>;
class IStrategy
{
public:
    IStrategy() = default;
    IStrategy(const IStrategy&) = default;
    IStrategy(IStrategy&&) = default;
    virtual ~IStrategy() = default;

    IStrategy& operator=(const IStrategy&) = default;
    IStrategy& operator=(IStrategy&&) = default;

    virtual GameStateWithMove getMiniMaxMove(const GameState&, EvaluationFunction, FigureColor, unsigned int maxDepth)
        const = 0;
};
