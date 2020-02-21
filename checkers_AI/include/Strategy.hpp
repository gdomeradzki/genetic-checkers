#pragma once
#include <functional>
#include "IStrategy.hpp"

class Strategy : public IStrategy
{
public:
    GameStateWithMove getMiniMaxMove(const GameState&, EvaluationFunction, FigureColor, unsigned int maxDepth)
        const override;
};
