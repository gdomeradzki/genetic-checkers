enable_testing()
include_directories(../googletest/include)
include_directories(../googlemock/include)

set (ut_mocks
    "../checkers_AI/include/tests/mocks/Mocks.hpp")
set (ut_source_files
    "../checkers_AI/tests/MetricsCalculatorTests.cpp"
    "../checkers_AI/tests/StrategyTests.cpp"
    "../checkers_AI/tests/HeuristicsTests.cpp"
    "../checkers_engine/tests/GameStateTests.cpp"
    "../checkers_engine/tests/GameControllerTests.cpp"
    "../checkers_engine/tests/GamePlayTests.cpp"
    "../checkers_learning/tests/GeneticAlgorithmTests.cpp")
add_executable(checkers_ut ${ut_mocks} ${ut_source_files})
target_link_libraries(checkers_ut gtest_main gmock_main checkers_ai checkers_engine checkers_learning_static)
set_target_properties(checkers_ut PROPERTIES
    CXX_STANDARD 17)
