#include "RandomEngine.hpp"
#include <iostream>
RandomEngine::RandomEngine(std::mt19937& gen) : gen{gen} {}

unsigned int RandomEngine::getRandomValue(unsigned int from, unsigned int to) const
{
    unsigned int distance = to - from;
    if (distance == 0)
    {
        return from;
    }
    unsigned int x = std::abs(dis(gen)) % distance + from;
    return x;
}
