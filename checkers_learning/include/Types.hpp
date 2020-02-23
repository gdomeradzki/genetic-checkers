#pragma once
#include <functional>

#include "IMetricsCalculator.hpp"

enum class GameStage
{
    EarlyGame,
    MidGame,
    LateGame
};
struct Gene
{
    Gene(GameStage gameStage, Metric metric) : metric(metric), gameStage(gameStage) {}
    Metric metric;
    GameStage gameStage;
    unsigned int factor{0};
};

constexpr auto gameStageNumber = 3;
constexpr auto metricsNumber = 22;
constexpr auto totalNumberOfGenes = gameStageNumber * metricsNumber;
struct Genotype
{
    std::array<Gene, totalNumberOfGenes> genes = {
        Gene{GameStage::EarlyGame, Metric::PawnsNumber},
        Gene{GameStage::EarlyGame, Metric::KingsNumber},
        Gene{GameStage::EarlyGame, Metric::SafePawns},
        Gene{GameStage::EarlyGame, Metric::SafeKings},
        Gene{GameStage::EarlyGame, Metric::MoveablePawns},
        Gene{GameStage::EarlyGame, Metric::MoveableKings},
        Gene{GameStage::EarlyGame, Metric::AggregatedDistanceToPromotionLine},
        Gene{GameStage::EarlyGame, Metric::NumberOfUnoccupiedFieldOnPromotionLine},
        Gene{GameStage::EarlyGame, Metric::DefenderFigures},
        Gene{GameStage::EarlyGame, Metric::AttackingFigures},
        Gene{GameStage::EarlyGame, Metric::CenterPawns},
        Gene{GameStage::EarlyGame, Metric::CenterKings},
        Gene{GameStage::EarlyGame, Metric::PawnsOnDiagonal},
        Gene{GameStage::EarlyGame, Metric::KingsOnDiagonal},
        Gene{GameStage::EarlyGame, Metric::PawnsOnDoubleDiagonal},
        Gene{GameStage::EarlyGame, Metric::KingsOnDoubleDiagonal},
        Gene{GameStage::EarlyGame, Metric::Triangle},
        Gene{GameStage::EarlyGame, Metric::Oreo},
        Gene{GameStage::EarlyGame, Metric::Bridge},
        Gene{GameStage::EarlyGame, Metric::Corner},
        Gene{GameStage::EarlyGame, Metric::KingCorner},
        Gene{GameStage::EarlyGame, Metric::Dog},
        Gene{GameStage::MidGame, Metric::PawnsNumber},
        Gene{GameStage::MidGame, Metric::KingsNumber},
        Gene{GameStage::MidGame, Metric::SafePawns},
        Gene{GameStage::MidGame, Metric::SafeKings},
        Gene{GameStage::MidGame, Metric::MoveablePawns},
        Gene{GameStage::MidGame, Metric::MoveableKings},
        Gene{GameStage::MidGame, Metric::AggregatedDistanceToPromotionLine},
        Gene{GameStage::MidGame, Metric::NumberOfUnoccupiedFieldOnPromotionLine},
        Gene{GameStage::MidGame, Metric::DefenderFigures},
        Gene{GameStage::MidGame, Metric::AttackingFigures},
        Gene{GameStage::MidGame, Metric::CenterPawns},
        Gene{GameStage::MidGame, Metric::CenterKings},
        Gene{GameStage::MidGame, Metric::PawnsOnDiagonal},
        Gene{GameStage::MidGame, Metric::KingsOnDiagonal},
        Gene{GameStage::MidGame, Metric::PawnsOnDoubleDiagonal},
        Gene{GameStage::MidGame, Metric::KingsOnDoubleDiagonal},
        Gene{GameStage::MidGame, Metric::Triangle},
        Gene{GameStage::MidGame, Metric::Oreo},
        Gene{GameStage::MidGame, Metric::Bridge},
        Gene{GameStage::MidGame, Metric::Corner},
        Gene{GameStage::MidGame, Metric::KingCorner},
        Gene{GameStage::MidGame, Metric::Dog},
        Gene{GameStage::LateGame, Metric::PawnsNumber},
        Gene{GameStage::LateGame, Metric::KingsNumber},
        Gene{GameStage::LateGame, Metric::SafePawns},
        Gene{GameStage::LateGame, Metric::SafeKings},
        Gene{GameStage::LateGame, Metric::MoveablePawns},
        Gene{GameStage::LateGame, Metric::MoveableKings},
        Gene{GameStage::LateGame, Metric::AggregatedDistanceToPromotionLine},
        Gene{GameStage::LateGame, Metric::NumberOfUnoccupiedFieldOnPromotionLine},
        Gene{GameStage::LateGame, Metric::DefenderFigures},
        Gene{GameStage::LateGame, Metric::AttackingFigures},
        Gene{GameStage::LateGame, Metric::CenterPawns},
        Gene{GameStage::LateGame, Metric::CenterKings},
        Gene{GameStage::LateGame, Metric::PawnsOnDiagonal},
        Gene{GameStage::LateGame, Metric::KingsOnDiagonal},
        Gene{GameStage::LateGame, Metric::PawnsOnDoubleDiagonal},
        Gene{GameStage::LateGame, Metric::KingsOnDoubleDiagonal},
        Gene{GameStage::LateGame, Metric::Triangle},
        Gene{GameStage::LateGame, Metric::Oreo},
        Gene{GameStage::LateGame, Metric::Bridge},
        Gene{GameStage::LateGame, Metric::Corner},
        Gene{GameStage::LateGame, Metric::KingCorner},
        Gene{GameStage::LateGame, Metric::Dog}};
    int fitness{0};
};

using OperationsDone = unsigned int;
using OperationsTotal = unsigned int;
using ProgressCallback = std::function<void(OperationsDone, OperationsTotal)>;
