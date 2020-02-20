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
