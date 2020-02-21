#pragma once

class IRandomEngine
{
public:
    IRandomEngine() = default;
    IRandomEngine(const IRandomEngine&) = default;
    IRandomEngine(IRandomEngine&&) = default;
    virtual ~IRandomEngine() = default;

    IRandomEngine& operator=(const IRandomEngine&) = default;
    IRandomEngine& operator=(IRandomEngine&&) = default;

    virtual unsigned int getRandomValue(unsigned int from, unsigned int to) const = 0;
};
