#pragma once

#include <chrono>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include "Types.hpp"

namespace Logger
{
std::string currentTimeFormatted();

template <typename... Args>
void log(Args&&... args)
{
    std::cout << currentTimeFormatted() << ": ";
    (std::cout << ... << std::forward<Args>(args)) << std::endl << std::flush; // NOLINT
}
template <typename... Args>
void logRepeatLine(Args&&... args)
{
    std::cout << "\r" << currentTimeFormatted() << ": ";
    (std::cout << ... << std::forward<Args>(args)) << "                       " << std::flush;
}
std::function<void(OperationsDone, OperationsTotal)> getProgressLogCallback(
    const std::chrono::high_resolution_clock::time_point& startTime);
} // namespace Logger

namespace File
{
void serialize(const std::string& filename, const Genotype& genotype);
} // namespace File
