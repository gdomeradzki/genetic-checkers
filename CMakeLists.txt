cmake_minimum_required(VERSION 3.10)

project(genetic-checkers)

add_subdirectory("${PROJECT_SOURCE_DIR}/googletest")

add_compile_options(-Wall -Wextra -pedantic -Werror -Wno-gnu-zero-variadic-macro-arguments)

add_subdirectory(checkers_engine)
add_subdirectory(checkers_frontend)
add_subdirectory(checkers_AI)
add_subdirectory(checkers_learning)
add_subdirectory(tests)
