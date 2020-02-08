project(checkers_engine)
set (headers
    "include/GameController.hpp"
    "include/GameState.hpp"
    "include/PawnState.hpp"
    "include/GamePlay.hpp"
    "include/Types.hpp")
set (sources
    "src/GameController.cpp"
    "src/GameState.cpp"
    "src/GamePlay.cpp")

add_library(checkers_engine SHARED ${sources} ${headers})
target_include_directories(checkers_engine PUBLIC "include")
set_target_properties(checkers_engine PROPERTIES
    CXX_STANDARD 17)

enable_testing()
set(checkers_engine_tests ${PROJECT_NAME}_test)
include_directories(../googletest/include)

set (test_sources
    "tests/GameStateTests.cpp"
    "tests/GameControllerTests.cpp"
    "tests/GamePlayTests.cpp")
add_executable(${checkers_engine_tests} ${test_sources})
target_link_libraries(${checkers_engine_tests} ${PROJECT_NAME} gtest_main)
set_target_properties(${checkers_engine_tests} PROPERTIES
    CXX_STANDARD 17)
