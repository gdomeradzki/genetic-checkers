project(checkers_ai)

set (headers
    "include/IMetricsCalculator.hpp"
    "include/MetricsCalculator.hpp"
    "include/IStrategy.hpp"
    "include/Strategy.hpp"
    "include/Heuristics.hpp")
set (sources
    "src/MetricsCalculator.cpp"
    "src/Strategy.cpp"
    "src/Heuristics.cpp")

add_library(checkers_ai ${sources} ${headers})
target_include_directories(checkers_ai PUBLIC "include")
target_include_directories(checkers_ai PUBLIC "../checkers_engine/include")
target_link_libraries(checkers_ai checkers_engine)
set_target_properties(checkers_ai PROPERTIES
    CXX_STANDARD 17)
