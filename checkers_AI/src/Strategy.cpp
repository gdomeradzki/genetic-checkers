#include "Strategy.hpp"

std::pair<int, GameStateWithMove> alphabeta(
    GameState gamestate,
    EvaluationFunction evalFunction,
    FigureColor callingPlayer,
    FigureColor currentPlayer,
    const int maxDepth,
    int currentDepth,
    int alpha,
    int beta)
{
    const GameController gameController(gamestate);
    const auto possibleMoves = gameController.getPossibleMoves(currentPlayer);
    if (currentDepth == maxDepth || possibleMoves.empty())
    {
        return {evalFunction(gamestate, callingPlayer), {}};
    }

    GameStateWithMove bestMove;
    if (callingPlayer == currentPlayer)
    {
        for (const auto& possibleMove : possibleMoves)
        {
            const auto ab = alphabeta(
                possibleMove.gameState,
                evalFunction,
                callingPlayer,
                FigureState::flipColor(currentPlayer),
                maxDepth,
                currentDepth + 1,
                alpha,
                beta);
            if (ab.first > alpha)
            {
                alpha = ab.first;
                bestMove = possibleMove;
            }
            if (alpha >= beta)
            {
                return {alpha, possibleMove};
            }
        }
        return {alpha, bestMove};
    }
    else
    {
        for (const auto& possibleMove : possibleMoves)
        {
            const auto ab = alphabeta(
                possibleMove.gameState,
                evalFunction,
                callingPlayer,
                FigureState::flipColor(currentPlayer),
                maxDepth,
                currentDepth + 1,
                alpha,
                beta);
            if (ab.first < beta)
            {
                beta = ab.first;
                bestMove = possibleMove;
            }

            if (alpha >= beta)
            {
                return {beta, possibleMove};
            }
        }
        return {beta, bestMove};
    }
}

GameStateWithMove Strategy::getRandomMove(const std::vector<GameStateWithMove>& possibleMoves) const
{
    int randomIndex = rand() % possibleMoves.size();
    return possibleMoves.at(randomIndex);
}

GameStateWithMove Strategy::getMiniMaxMove(
    const GameState& gameState,
    EvaluationFunction evalFunction,
    FigureColor figureColor,
    int maxDepth) const
{
    return alphabeta(
               gameState,
               std::move(evalFunction),
               figureColor,
               figureColor,
               maxDepth,
               0,
               std::numeric_limits<int>::min(),
               std::numeric_limits<int>::max())
        .second;
}
