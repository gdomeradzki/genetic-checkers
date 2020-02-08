#include "RandomEngine.hpp"
#include <iostream>
RandomEngine::RandomEngine(std::mt19937& gen) : gen{gen} {}

int RandomEngine::getRandomValue(int from, int to) const
{
    int distance = to - from;
    if (distance == 0)
    {
        return from;
    }
    if (to < from) std::cout << " from:" << from << " to: " << to << std::endl;
    int x = dis(gen) % distance + from;
    return x;
}
