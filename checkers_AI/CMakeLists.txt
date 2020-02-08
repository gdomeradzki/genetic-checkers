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

enable_testing()
set(checkers_ai_tests ${PROJECT_NAME}_test)
include_directories(../googletest/include)
include_directories(../googlemock/include)

set (test_mocks
    "include/tests/mocks/Mocks.hpp")
set (test_sources
    "tests/MetricsCalculatorTests.cpp"
    "tests/StrategyTests.cpp"
    "tests/HeuristicsTests.cpp")
add_executable(${checkers_ai_tests} ${test_mocks} ${test_sources})
target_link_libraries(${checkers_ai_tests} ${PROJECT_NAME} gtest_main gmock_main)
set_target_properties(${checkers_ai_tests} PROPERTIES
    CXX_STANDARD 17)



