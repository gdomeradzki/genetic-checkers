#include "GameController.hpp"

#include <algorithm>
#include <iostream>

namespace
{
inline bool isInBoard(Position position)
{
    return position.col >= 0 && position.col < BOARD_SIZE && position.row >= 0 && position.row < BOARD_SIZE;
}
} // namespace

GameController::GameController(const GameState& gameState) : m_gameState(gameState) {}

std::vector<GameStateWithMove> GameController::getPossibleMoves(FigureColor color) const
{
    std::vector<GameStateWithMove> jumps;
    std::vector<GameStateWithMove> moves;

    const auto& pawns = m_gameState.pawns(color);

    bool foundAnyJump = false;
    for (const auto& pawn : pawns)
    {
        const auto pawnJumps = getAvailableJumps(pawn);
        jumps.insert(jumps.end(), pawnJumps.begin(), pawnJumps.end());
        if (!foundAnyJump)
        {
            if (jumps.empty())
            {
                const auto pawnMoves = getAvailableMoves(pawn);
                moves.insert(moves.end(), pawnMoves.begin(), pawnMoves.end());
            }
            else
            {
                foundAnyJump = true;
            }
        }
    }

    if (foundAnyJump)
    {
        return jumps;
    }

    return moves;
}

GameResult GameController::gameResult(FigureColor color) const
{
    if (getPossibleMoves(color).empty())
    {
        return color == FigureColor::White ? GameResult::BlackWin : GameResult::WhiteWin;
    }

    return GameResult::GameOn;
}

std::vector<GameStateWithMove> GameController::getAvailableJumps(const Figure& pawn) const
{
    std::vector<GameStateWithMove> jumps;
    findJump(m_gameState, Move{}, pawn, jumps);
    if (jumps.empty())
    {
        return jumps;
    }
    std::sort(jumps.begin(), jumps.end(), [](const auto& first, const auto& second) {
        return first.move.size() < second.move.size();
    });
    const auto longestJumpSize = jumps.back().move.size();
    const auto beginOfLongestJumps =
        std::lower_bound(jumps.cbegin(), jumps.cend(), longestJumpSize, [](const auto& first, const auto& second) {
            return first.move.size() < second;
        });

    return {beginOfLongestJumps, jumps.cend()};
}

void GameController::findJump(
    GameState gameState,
    Move jump,
    const Figure jumpingPawn,
    std::vector<GameStateWithMove>& allJumps) const
{
    jump.push_back(jumpingPawn.position);
    bool foundJump{false};
    if (jumpingPawn.state.type != FigureType::King)
    {
        constexpr auto pawnBeatOverOponent = 2;
        for (const auto rowJump : {-pawnBeatOverOponent, pawnBeatOverOponent})
        {
            for (const auto colJump : {-pawnBeatOverOponent, pawnBeatOverOponent})
            {
                const auto newPosition =
                    Position{jumpingPawn.position.row + rowJump, jumpingPawn.position.col + colJump};
                if (isInBoard(newPosition))
                {
                    if (gameState.isFree(newPosition))
                    {
                        const auto positionBetween = Position{jumpingPawn.position.row + rowJump / pawnBeatOverOponent,
                                                              jumpingPawn.position.col + colJump / pawnBeatOverOponent};
                        if (!gameState.isFree(positionBetween))
                        {
                            if (gameState.pawnAtPosition(positionBetween).color != jumpingPawn.state.color)
                            {
                                auto newGameState = gameState;
                                newGameState.removePawn(positionBetween);
                                newGameState.movePawn(jumpingPawn.position, newPosition);
                                auto newJumpingPawn = jumpingPawn;
                                newJumpingPawn.position = newPosition;
                                findJump(newGameState, jump, newJumpingPawn, allJumps);
                                foundJump = true;
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        for (const auto rowWayFactor : {-1, 1})
        {
            for (const auto colWayFactor : {-1, 1})
            {
                for (int i = 1; i < BOARD_SIZE; i++)
                {
                    const auto beatenPosition = Position{jumpingPawn.position.row + i * rowWayFactor,
                                                         jumpingPawn.position.col + i * colWayFactor};
                    if (!isInBoard(beatenPosition))
                    {
                        break;
                    }

                    if (gameState.isFree(beatenPosition))
                    {
                        continue;
                    }
                    if (gameState.pawnAtPosition(beatenPosition).color == jumpingPawn.state.color)
                    {
                        break;
                    }
                    bool beatenBlocked{false};
                    for (int j = i + 1; j < BOARD_SIZE; j++)
                    {
                        const auto positionBeyond = Position{jumpingPawn.position.row + j * rowWayFactor,
                                                             jumpingPawn.position.col + j * colWayFactor};
                        if (!isInBoard(positionBeyond) || !gameState.isFree(positionBeyond))
                        {
                            beatenBlocked = true;
                            break;
                        }
                        auto newGameState = gameState;
                        newGameState.removePawn(beatenPosition);
                        newGameState.movePawn(jumpingPawn.position, positionBeyond);
                        auto newJumpingPawn = jumpingPawn;
                        newJumpingPawn.position = positionBeyond;
                        findJump(newGameState, jump, newJumpingPawn, allJumps);
                        foundJump = true;
                    }
                    if (beatenBlocked)
                    {
                        break;
                    }
                }
            }
        }
    }
    if (jump.size() > 1 && !foundJump)
    {
        if (isKingChange(jumpingPawn.state, jumpingPawn.position))
        {
            gameState.changePawnType(jumpingPawn.position, FigureType::King);
        }
        allJumps.push_back({gameState, jump});
    }
}
std::vector<GameStateWithMove> GameController::getAvailableMoves(const Figure& pawn) const
{
    std::vector<GameStateWithMove> movesWithGameState;
    if (pawn.state.type != FigureType::King)
    {
        constexpr auto pawnMoveRange = 1;
        const auto rowDirectionMove = pawn.state.color == FigureColor::White ? pawnMoveRange : -pawnMoveRange;

        for (const auto colMove : {-pawnMoveRange, pawnMoveRange})
        {
            const auto movePosition = Position{pawn.position.row + rowDirectionMove, pawn.position.col + colMove};
            if (isInBoard(movePosition))
            {
                Move move{pawn.position};
                if (m_gameState.isFree(movePosition))
                {
                    auto gameStateCopy = m_gameState;
                    gameStateCopy.movePawn(pawn.position, movePosition);
                    if (isKingChange(pawn.state, movePosition))
                    {
                        gameStateCopy.changePawnType(movePosition, FigureType::King);
                    }
                    move.push_back(movePosition);
                    movesWithGameState.push_back({gameStateCopy, move});
                }
            }
        }
    }
    else
    {
        for (const auto rowWayFactor : {-1, 1})
        {
            for (const auto colWayFactor : {-1, 1})
            {
                for (int i = 1; i < BOARD_SIZE; i++)
                {
                    const auto movePosition =
                        Position{pawn.position.row + rowWayFactor * i, pawn.position.col + colWayFactor * i};
                    Move move{pawn.position, movePosition};
                    if (!isInBoard(movePosition) || !m_gameState.isFree(movePosition))
                    {
                        break;
                    }
                    if (m_gameState.isFree(movePosition))
                    {
                        auto gameStateCopy = m_gameState;
                        gameStateCopy.movePawn(pawn.position, movePosition);
                        movesWithGameState.push_back({gameStateCopy, move});
                    }
                }
            }
        }
    }

    return movesWithGameState;
}

bool GameController::isKingChange(FigureState pawnState, Position position) const
{
    if (pawnState.type == FigureType::King)
    {
        return false;
    }
    if (pawnState.color == FigureColor::White)
    {
        if (position.row == BOARD_SIZE - 1)
        {
            return true;
        }
    }
    else
    {
        if (position.row == 0)
        {
            return true;
        }
    }
    return false;
}
