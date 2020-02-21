#pragma once

#include <limits.h>
#include <random>
#include "IRandomEngine.hpp"
static std::uniform_int_distribution<> dis(0, INT_MAX);
class RandomEngine : public IRandomEngine
{
public:
    explicit RandomEngine(std::mt19937& gen);
    unsigned int getRandomValue(unsigned int from, unsigned int to) const override;

private:
    std::mt19937& gen;
};
