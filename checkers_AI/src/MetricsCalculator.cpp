#include "MetricsCalculator.hpp"

#include <algorithm>
#include <cmath>
#include <functional>

#include "GameController.hpp"

int calcFiguresRatio(int maxPossiblePoints, int playerPoints, int opponentPoints)
{
    constexpr auto halfRange = Calculator::valueRange / 2;

    if (playerPoints == 0)
    {
        const double playerFactor = static_cast<double>(opponentPoints) / static_cast<double>(maxPossiblePoints);
        return halfRange - halfRange * playerFactor;
    }

    if (opponentPoints == 0)
    {
        const double opponentFactor = static_cast<double>(playerPoints) / static_cast<double>(maxPossiblePoints);
        return halfRange + halfRange * opponentFactor;
    }

    if (playerPoints > opponentPoints)
    {
        const double playerFactor = (static_cast<double>(playerPoints - 1) / static_cast<double>(opponentPoints)) /
            static_cast<double>(maxPossiblePoints);
        return halfRange + halfRange * playerFactor;
    }
    else if (playerPoints < opponentPoints)
    {
        const double playerFactor = (static_cast<double>(opponentPoints - 1) / static_cast<double>(playerPoints)) /
            static_cast<double>(maxPossiblePoints);
        return halfRange - halfRange * playerFactor;
    }
    else
    {
        return halfRange;
    }
}
void constrainFigures(Figures& figures, std::function<bool(const Figure&)> removeIfPredicate)
{
    figures.erase(std::remove_if(figures.begin(), figures.end(), removeIfPredicate), figures.end());
}

int pawnsNumberMetric(Figures player, Figures opponent)
{
    constrainFigures(player, [](const Figure& pawn) { return pawn.state.type != FigureType::Pawn; });
    constrainFigures(opponent, [](const Figure& pawn) { return pawn.state.type != FigureType::Pawn; });

    return calcFiguresRatio(totalPlayerFiguresNumber, player.size(), opponent.size());
}
int kingsNumberMetric(Figures player, Figures opponent)
{
    constrainFigures(player, [](const Figure& pawn) { return pawn.state.type != FigureType::King; });
    constrainFigures(opponent, [](const Figure& pawn) { return pawn.state.type != FigureType::King; });

    return calcFiguresRatio(totalPlayerFiguresNumber, player.size(), opponent.size());
}

bool isSafePosition(Position position)
{
    return position.row == 0 || position.row == (boardSize - 1) || position.col == 0 || position.col == (boardSize - 1);
}

int safePawnsMetric(Figures player, Figures opponent)
{
    constrainFigures(player, [](const Figure& pawn) {
        return pawn.state.type != FigureType::Pawn || !isSafePosition(pawn.position);
    });
    constrainFigures(opponent, [](const Figure& pawn) {
        return pawn.state.type != FigureType::Pawn || !isSafePosition(pawn.position);
    });
    constexpr auto maxPossibleSafePawns = 10;
    return calcFiguresRatio(maxPossibleSafePawns, player.size(), opponent.size());
}
int safeKingsMetric(Figures player, Figures opponent)
{
    constrainFigures(player, [](const Figure& pawn) {
        return pawn.state.type != FigureType::King || !isSafePosition(pawn.position);
    });
    constrainFigures(opponent, [](const Figure& pawn) {
        return pawn.state.type != FigureType::King || !isSafePosition(pawn.position);
    });

    return calcFiguresRatio(totalPlayerFiguresNumber, player.size(), opponent.size());
}

bool isCenterPosition(Position position)
{
    return position.row == boardSize / 2 || position.row == (boardSize / 2 - 1);
}

int centerPawnsMetric(Figures player, Figures opponent)
{
    constrainFigures(player, [](const Figure& pawn) {
        return pawn.state.type != FigureType::Pawn || !isCenterPosition(pawn.position);
    });
    constrainFigures(opponent, [](const Figure& pawn) {
        return pawn.state.type != FigureType::Pawn || !isCenterPosition(pawn.position);
    });

    return calcFiguresRatio(totalPlayerFiguresNumber, player.size(), opponent.size());
}
int centerKingsMetric(Figures player, Figures opponent)
{
    constrainFigures(player, [](const Figure& pawn) {
        return pawn.state.type != FigureType::King || !isCenterPosition(pawn.position);
    });
    constrainFigures(opponent, [](const Figure& pawn) {
        return pawn.state.type != FigureType::King || !isCenterPosition(pawn.position);
    });

    return calcFiguresRatio(totalPlayerFiguresNumber, player.size(), opponent.size());
}

int moveableFiguresMetric(
    const GameState& gameState,
    const std::vector<GameStateWithMove>& playerMoves,
    const std::vector<GameStateWithMove>& opponentMoves,
    FigureType type)
{
    std::set<Position> playersPosition;
    std::set<Position> opponentPosition;
    for (const auto& move : playerMoves)
    {
        if (gameState.pawnAtPosition(move.move.front()).type == type)
        {
            playersPosition.insert(move.move.front());
        }
    }
    for (const auto& move : opponentMoves)
    {
        if (gameState.pawnAtPosition(move.move.front()).type == type)
        {
            opponentPosition.insert(move.move.front());
        }
    }
    return calcFiguresRatio(totalPlayerFiguresNumber, playersPosition.size(), opponentPosition.size());
}
int moveablePawnsMetric(const GameState& gameState, FigureColor player, FigureColor opponent)
{
    GameController gameController(gameState);
    return moveableFiguresMetric(
        gameState,
        gameController.getPossibleMoves(player),
        gameController.getPossibleMoves(opponent),
        FigureType::Pawn);
}
int moveableKingsMetric(const GameState& gameState, FigureColor player, FigureColor opponent)
{
    GameController gameController(gameState);
    return moveableFiguresMetric(
        gameState,
        gameController.getPossibleMoves(player),
        gameController.getPossibleMoves(opponent),
        FigureType::King);
}

int aggregatedDistanceToPromotionLineMetric(Figures player, Figures opponent)
{
    Figures whitePawns;
    Figures blackPawns;
    const bool isPlayerWhiteColor = player.at(0).state.color == FigureColor::White;
    if (isPlayerWhiteColor)
    {
        whitePawns = std::move(player);
        blackPawns = std::move(opponent);
    }
    else
    {
        whitePawns = std::move(opponent);
        blackPawns = std::move(player);
    }
    constrainFigures(whitePawns, [](const Figure& pawn) { return pawn.state.type != FigureType::Pawn; });
    constrainFigures(blackPawns, [](const Figure& pawn) { return pawn.state.type != FigureType::Pawn; });

    int playerTotalSum = 0;
    int maxDistanceSum = 0;
    int minDistanceSum = 0;
    constexpr auto pawnsPerRow = boardSize / 2;
    for (size_t i = 0; i < whitePawns.size(); i++)
    {
        minDistanceSum += (i / pawnsPerRow);
        playerTotalSum += whitePawns[i].position.row;
        maxDistanceSum += (boardSize - 2) - (i / pawnsPerRow);
    }
    const double whiteFactor =
        static_cast<double>(playerTotalSum - minDistanceSum) / static_cast<double>(maxDistanceSum - minDistanceSum);

    maxDistanceSum = 0;
    playerTotalSum = 0;
    minDistanceSum = 0;
    for (size_t i = 0; i < blackPawns.size(); i++)
    {
        minDistanceSum += (i / pawnsPerRow) + 1;
        playerTotalSum += blackPawns[i].position.row;
        maxDistanceSum += (boardSize - 1) - (i / pawnsPerRow);
    }
    const double blackFactor =
        static_cast<double>(maxDistanceSum - playerTotalSum) / static_cast<double>(maxDistanceSum - minDistanceSum);

    if (isPlayerWhiteColor)
    {
        return calcFiguresRatio(
            totalPlayerFiguresNumber,
            round(whiteFactor * totalPlayerFiguresNumber),
            round(blackFactor * totalPlayerFiguresNumber));
    }
    else
    {
        return calcFiguresRatio(
            totalPlayerFiguresNumber,
            round(blackFactor * totalPlayerFiguresNumber),
            round(whiteFactor * totalPlayerFiguresNumber));
    }
}
int numberOfUnoccupiedFieldsOnPromotionLineMetric(FigureColor playerColor, const GameState& gameState)
{
    constexpr auto maxPossibleUnoccupiedFields = boardSize / 2;
    int whitePlayerUnoccupiedFields = 0;
    int blackPlayerUnoccupiedFields = 0;
    for (int i = 0; i < boardSize; i += 2)
    {
        if (gameState.isFree(Position{boardSize - 1, i + 1}))
        {
            whitePlayerUnoccupiedFields++;
        }
        if (gameState.isFree(Position{0, i}))
        {
            blackPlayerUnoccupiedFields++;
        }
    }
    if (playerColor == FigureColor::White)
    {
        return calcFiguresRatio(maxPossibleUnoccupiedFields, whitePlayerUnoccupiedFields, blackPlayerUnoccupiedFields);
    }
    else
    {
        return calcFiguresRatio(maxPossibleUnoccupiedFields, blackPlayerUnoccupiedFields, whitePlayerUnoccupiedFields);
    }
}
int defenderFiguresMetric(Figures player, Figures opponent)
{
    const auto isPlayerWhite = player.at(0).state.color == FigureColor::White;
    if (isPlayerWhite)
    {
        constrainFigures(player, [](const Figure& figure) { return figure.position.row > (boardSize / 2 - 2); });
        constrainFigures(opponent, [](const Figure& figure) { return figure.position.row < (boardSize / 2 + 1); });
    }
    else
    {
        constrainFigures(opponent, [](const Figure& figure) { return figure.position.row > (boardSize / 2 - 2); });
        constrainFigures(player, [](const Figure& figure) { return figure.position.row < (boardSize / 2 + 1); });
    }
    return calcFiguresRatio(totalPlayerFiguresNumber, player.size(), opponent.size());
}
int attackingFiguresMetric(Figures player, Figures opponent)
{
    const auto isPlayerWhite = player.at(0).state.color == FigureColor::White;
    if (!isPlayerWhite)
    {
        constrainFigures(player, [](const Figure& figure) { return figure.position.row > (boardSize / 2 - 2); });
        constrainFigures(opponent, [](const Figure& figure) { return figure.position.row < (boardSize / 2 + 1); });
    }
    else
    {
        constrainFigures(opponent, [](const Figure& figure) { return figure.position.row > (boardSize / 2 - 2); });
        constrainFigures(player, [](const Figure& figure) { return figure.position.row < (boardSize / 2 + 1); });
    }
    return calcFiguresRatio(totalPlayerFiguresNumber, player.size(), opponent.size());
}

bool isDiagonalPosition(Position position)
{
    return position.row == position.col;
}
int figuresOnDiagonalMetric(Figures player, Figures opponent, FigureType type, int maxNumberOfFiguresOnDiagonal)
{
    constrainFigures(player, [type](const Figure& figure) {
        return !isDiagonalPosition(figure.position) || figure.state.type != type;
    });
    constrainFigures(opponent, [type](const Figure& figure) {
        return !isDiagonalPosition(figure.position) || figure.state.type != type;
    });

    return calcFiguresRatio(maxNumberOfFiguresOnDiagonal, player.size(), opponent.size());
}
int pawnsOnDiagonalMetric(Figures player, Figures opponent)
{
    return figuresOnDiagonalMetric(player, opponent, FigureType::Pawn, boardSize - 1);
}
int kingsOnDiagonalMetric(Figures player, Figures opponent)
{
    return figuresOnDiagonalMetric(player, opponent, FigureType::King, boardSize);
}

bool isDoubleDiagonalPosition(Position position)
{
    return (position.row == position.col + 2) || (position.row == position.col - 2);
}
int figuresOnDoubleDiagonalMetric(Figures player, Figures opponent, FigureType type, int maxNumberOfFiguresOnDiagonal)
{
    constrainFigures(player, [type](const Figure& figure) {
        return !isDoubleDiagonalPosition(figure.position) || figure.state.type != type;
    });
    constrainFigures(opponent, [type](const Figure& figure) {
        return !isDoubleDiagonalPosition(figure.position) || figure.state.type != type;
    });

    return calcFiguresRatio(maxNumberOfFiguresOnDiagonal, player.size(), opponent.size());
}
int pawnsOnDoubleDiagonalMetric(Figures player, Figures opponent)
{
    return figuresOnDoubleDiagonalMetric(
        std::move(player), std::move(opponent), FigureType::Pawn, totalPlayerFiguresNumber - 1);
}
int kingsOnDoubleDiagonalMetric(Figures player, Figures opponent)
{
    return figuresOnDoubleDiagonalMetric(
        std::move(player), std::move(opponent), FigureType::King, totalPlayerFiguresNumber);
}

int patternMetric(FigureColor playerColor, bool whitePatternSatisfied, bool blackPatternSatisfied)
{
    if (playerColor == FigureColor::White)
    {
        return calcFiguresRatio(1, whitePatternSatisfied ? 1 : 0, blackPatternSatisfied ? 1 : 0);
    }
    else
    {
        return calcFiguresRatio(1, blackPatternSatisfied ? 1 : 0, whitePatternSatisfied ? 1 : 0);
    }
}
int triangleMetric(const GameState& gameState, FigureColor playerColor)
{
    const bool whitePawnsAvailable =
        !gameState.isFree({0, 4}) && !gameState.isFree({0, 6}) && !gameState.isFree({1, 5});
    const bool blackPawnsAvailable =
        !gameState.isFree({7, 1}) && !gameState.isFree({7, 3}) && !gameState.isFree({6, 2});

    if (whitePawnsAvailable && blackPawnsAvailable)
    {
        const bool isWhiteTriangle =
            (gameState.pawnAtPosition({0, 4}).color == FigureColor::White &&
             gameState.pawnAtPosition({0, 6}).color == FigureColor::White &&
             gameState.pawnAtPosition({1, 5}).color == FigureColor::White);
        const bool isBlackTriangle =
            (gameState.pawnAtPosition({7, 1}).color == FigureColor::Black &&
             gameState.pawnAtPosition({7, 3}).color == FigureColor::Black &&
             gameState.pawnAtPosition({6, 2}).color == FigureColor::Black);

        return patternMetric(playerColor, isWhiteTriangle, isBlackTriangle);
    }

    return patternMetric(playerColor, whitePawnsAvailable, blackPawnsAvailable);
}
int oreoMetric(const GameState& gameState, FigureColor playerColor)
{
    const bool whitePawnsAvailable =
        !gameState.isFree({0, 2}) && !gameState.isFree({0, 4}) && !gameState.isFree({1, 3});
    const bool blackPawnsAvailable =
        !gameState.isFree({7, 3}) && !gameState.isFree({7, 5}) && !gameState.isFree({6, 4});

    if (whitePawnsAvailable && blackPawnsAvailable)
    {
        const bool isWhiteOreo =
            (gameState.pawnAtPosition({0, 2}).color == FigureColor::White &&
             gameState.pawnAtPosition({0, 4}).color == FigureColor::White &&
             gameState.pawnAtPosition({1, 3}).color == FigureColor::White);
        const bool isBlackOreo =
            (gameState.pawnAtPosition({7, 3}).color == FigureColor::Black &&
             gameState.pawnAtPosition({7, 5}).color == FigureColor::Black &&
             gameState.pawnAtPosition({6, 4}).color == FigureColor::Black);

        return patternMetric(playerColor, isWhiteOreo, isBlackOreo);
    }

    return patternMetric(playerColor, whitePawnsAvailable, blackPawnsAvailable);
}
int bridgeMetric(const GameState& gameState, FigureColor playerColor)
{
    const bool whitePawnsAvailable = !gameState.isFree({0, 2}) && !gameState.isFree({0, 6});
    const bool blackPawnsAvailable = !gameState.isFree({7, 1}) && !gameState.isFree({7, 5});

    if (whitePawnsAvailable && blackPawnsAvailable)
    {
        const bool isWhiteBridge =
            (gameState.pawnAtPosition({0, 2}).color == FigureColor::White &&
             gameState.pawnAtPosition({0, 6}).color == FigureColor::White);
        const bool isBlackBridge =
            (gameState.pawnAtPosition({7, 1}).color == FigureColor::Black &&
             gameState.pawnAtPosition({7, 5}).color == FigureColor::Black);

        return patternMetric(playerColor, isWhiteBridge, isBlackBridge);
    }

    return patternMetric(playerColor, whitePawnsAvailable, blackPawnsAvailable);
}
int pawnCornerMetric(const GameState& gameState, FigureColor playerColor)
{
    const bool whitePawnsAvailable = !gameState.isFree({0, 0});
    const bool blackPawnsAvailable = !gameState.isFree({7, 7});

    if (whitePawnsAvailable && blackPawnsAvailable)
    {
        const bool isWhiteCorner = gameState.pawnAtPosition({0, 0}).color == FigureColor::White &&
            gameState.pawnAtPosition({0, 0}).type == FigureType::Pawn;
        const bool isBlackCorner = gameState.pawnAtPosition({7, 7}).color == FigureColor::Black &&
            gameState.pawnAtPosition({7, 7}).type == FigureType::Pawn;

        return patternMetric(playerColor, isWhiteCorner, isBlackCorner);
    }

    return patternMetric(playerColor, whitePawnsAvailable, blackPawnsAvailable);
}
int kingCornerMetric(const GameState& gameState, FigureColor playerColor)
{
    const bool whitePawnsAvailable = !gameState.isFree({0, 0});
    const bool blackPawnsAvailable = !gameState.isFree({7, 7});

    if (whitePawnsAvailable && blackPawnsAvailable)
    {
        const bool isWhiteCorner = gameState.pawnAtPosition({7, 7}).color == FigureColor::White &&
            gameState.pawnAtPosition({7, 7}).type == FigureType::King;
        const bool isBlackCorner = gameState.pawnAtPosition({0, 0}).color == FigureColor::Black &&
            gameState.pawnAtPosition({0, 0}).type == FigureType::King;

        return patternMetric(playerColor, isWhiteCorner, isBlackCorner);
    }

    return patternMetric(playerColor, whitePawnsAvailable, blackPawnsAvailable);
}
int dogMetric(const GameState& gameState, FigureColor playerColor)
{
    const bool whitePawnsAvailable = !gameState.isFree({1, 7}) && !gameState.isFree({0, 6});
    const bool blackPawnsAvailable = !gameState.isFree({7, 1}) && !gameState.isFree({6, 0});

    if (whitePawnsAvailable && blackPawnsAvailable)
    {
        const bool isWhiteDog = gameState.pawnAtPosition({0, 6}).color == FigureColor::White &&
            gameState.pawnAtPosition({1, 7}).color == FigureColor::Black &&
            gameState.pawnAtPosition({0, 6}).type == FigureType::Pawn &&
            gameState.pawnAtPosition({1, 7}).type == FigureType::Pawn;

        const bool isBlackDog = gameState.pawnAtPosition({6, 0}).color == FigureColor::White &&
            gameState.pawnAtPosition({7, 1}).color == FigureColor::Black &&
            gameState.pawnAtPosition({6, 0}).type == FigureType::Pawn &&
            gameState.pawnAtPosition({7, 1}).type == FigureType::Pawn;

        return patternMetric(playerColor, isWhiteDog, isBlackDog);
    }

    return patternMetric(playerColor, whitePawnsAvailable, blackPawnsAvailable);
}

int MetricsCalculator::evaluate(
    const std::set<MetricFactor>& metricWithFactors,
    const GameState& gameState,
    FigureColor playerColor) const
{
    const FigureColor opponentColor = FigureState::flipColor(playerColor);
    const auto playerFigures = gameState.pawns(playerColor);
    const auto opponentFigures = gameState.pawns(opponentColor);
    if (metricWithFactors.empty())
    {
        return Calculator::minValue;
    }
    if (playerFigures.empty())
    {
        return Calculator::minValue;
    }
    if (opponentFigures.empty())
    {
        return Calculator::maxValue;
    }

    const auto metricsNumber = metricWithFactors.size();
    int totalSum = 0;

    for (const auto& metricFactor : metricWithFactors)
    {
        int metricValue = 0;
        switch (metricFactor.metric)
        {
            case Metric::PawnsNumber:
                metricValue = pawnsNumberMetric(playerFigures, opponentFigures);
                break;
            case Metric::KingsNumber:
                metricValue = kingsNumberMetric(playerFigures, opponentFigures);
                break;
            case Metric::SafePawns:
                metricValue = safePawnsMetric(playerFigures, opponentFigures);
                break;
            case Metric::SafeKings:
                metricValue = safeKingsMetric(playerFigures, opponentFigures);
                break;
            case Metric::MoveablePawns:
                metricValue = moveablePawnsMetric(gameState, playerColor, opponentColor);
                break;
            case Metric::MoveableKings:
                metricValue = moveableKingsMetric(gameState, playerColor, opponentColor);
                break;
            case Metric::AggregatedDistanceToPromotionLine:
                metricValue = aggregatedDistanceToPromotionLineMetric(playerFigures, opponentFigures);
                break;
            case Metric::NumberOfUnoccupiedFieldOnPromotionLine:
                metricValue = numberOfUnoccupiedFieldsOnPromotionLineMetric(playerColor, gameState);
                break;
            case Metric::DefenderFigures:
                metricValue = defenderFiguresMetric(playerFigures, opponentFigures);
                break;
            case Metric::AttackingFigures:
                metricValue = attackingFiguresMetric(playerFigures, opponentFigures);
                break;
            case Metric::CenterPawns:
                metricValue = centerPawnsMetric(playerFigures, opponentFigures);
                break;
            case Metric::CenterKings:
                metricValue = centerKingsMetric(playerFigures, opponentFigures);
                break;
            case Metric::PawnsOnDiagonal:
                metricValue = pawnsOnDiagonalMetric(playerFigures, opponentFigures);
                break;
            case Metric::KingsOnDiagonal:
                metricValue = kingsOnDiagonalMetric(playerFigures, opponentFigures);
                break;
            case Metric::PawnsOnDoubleDiagonal:
                metricValue = pawnsOnDoubleDiagonalMetric(playerFigures, opponentFigures);
                break;
            case Metric::KingsOnDoubleDiagonal:
                metricValue = kingsOnDoubleDiagonalMetric(playerFigures, opponentFigures);
                break;
            case Metric::Triangle:
                metricValue = triangleMetric(gameState, playerColor);
                break;
            case Metric::Oreo:
                metricValue = oreoMetric(gameState, playerColor);
                break;
            case Metric::Bridge:
                metricValue = bridgeMetric(gameState, playerColor);
                break;
            case Metric::Corner:
                metricValue = pawnCornerMetric(gameState, playerColor);
                break;
            case Metric::KingCorner:
                metricValue = kingCornerMetric(gameState, playerColor);
                break;
            case Metric::Dog:
                metricValue = dogMetric(gameState, playerColor);
                break;
            default:
                throw std::runtime_error("Metric not handled!");
        }
        metricValue *= metricFactor.factor;
        totalSum += metricValue;
    }
    return totalSum / metricsNumber;
}
