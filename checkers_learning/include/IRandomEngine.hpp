#pragma once

class IRandomEngine
{
public:
    virtual ~IRandomEngine() = default;
    virtual int getRandomValue(int from, int to) const = 0;
};
