project(checkers_frontend)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set (headers
    "include/FrontBoard.hpp"
    "include/FrontendController.hpp"
    "include/MainWindow.hpp"
    "include/Helpers.hpp")
set (sources
    "src/FrontBoard.cpp"
    "src/FrontendController.cpp"
    "src/MainWindow.cpp"
    "src/Helpers.cpp"
    "src/main.cpp")
set (resources
    "resources/resources.qrc")
set (uis
    "uis/FrontBoard.ui"
    "uis/MainWindow.ui")
set(CMAKE_AUTOUIC_SEARCH_PATHS "uis")
qt5_add_resources(resources)

add_executable(checkers_frontend ${sources} ${headers} ${uis} ${resources})
target_include_directories(checkers_frontend PUBLIC "include")
target_include_directories(checkers_frontend PUBLIC "../checkers_engine/include")
target_include_directories(checkers_frontend PUBLIC "../checkers_AI/include")
set_target_properties(checkers_frontend PROPERTIES
    CXX_STANDARD 17)

target_link_libraries(checkers_frontend checkers_engine)
target_link_libraries(checkers_frontend checkers_ai)
target_link_libraries(checkers_frontend Qt5::Widgets)
