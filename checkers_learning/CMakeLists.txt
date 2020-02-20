project(checkers_learning)

set (headers
    "include/GeneticAlgorithm.hpp"
    "include/IParrarelGameplay.hpp"
    "include/IRandomEngine.hpp"
    "include/Helpers.hpp"
    "include/Types.hpp")
set (sources
    "src/GeneticAlgorithm.cpp"
    "src/Helpers.cpp")

set(headers_main
    "include/ParrarelGameplay.hpp"
    "include/RandomEngine.hpp")
set(sources_main
    "src/main.cpp"
    "src/ParrarelGameplay.cpp"
    "src/RandomEngine.cpp")
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

add_library(checkers_learning_static STATIC ${sources} ${headers})
target_include_directories(checkers_learning_static PUBLIC "include")
target_include_directories(checkers_learning_static PUBLIC "../checkers_AI/include")
target_include_directories(checkers_learning_static PUBLIC "../checkers_engine/include")
target_link_libraries(checkers_learning_static checkers_engine checkers_ai)
target_link_libraries(checkers_learning_static Threads::Threads)
set_target_properties(checkers_learning_static PROPERTIES
    CXX_STANDARD 17)

add_executable(checkers_learning ${sources} ${sources_main} ${headers})
target_include_directories(checkers_learning PUBLIC "include")
target_include_directories(checkers_learning PUBLIC "../checkers_engine/include")
target_include_directories(checkers_learning PUBLIC "../checkers_AI/include")
target_link_libraries(checkers_learning checkers_engine checkers_ai)
target_link_libraries(checkers_learning Threads::Threads)
set_target_properties(checkers_learning PROPERTIES
    CXX_STANDARD 17)




