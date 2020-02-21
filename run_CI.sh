#!/bin/bash

clear_dir(){
    show_yellow_text "CI dir cleanup..."
    rm -rf build_ci
}
build_ci(){
    mkdir build_ci
    cd build_ci
    show_yellow_text "Build starts..."
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DQt5_DIR=$1 ..
    if [ $? -ne 0 ]; then
        show_red_text "CMake failed."
        clear_dir
        exit 1
    fi
    make -j$(nproc)
    if [ $? -ne 0 ]; then
        show_red_text "Build failed."
        clear_dir
        exit 1
    fi
    show_green_text "Build succeed!"
    cd ..
}
show_green_text(){
    echo -e "\e[32m\e[1m$1"
    echo -e "\e[0m"
}
show_red_text(){
    echo -e "\e[31m\e[1m$1"
    echo -e "\e[0m"
}
show_yellow_text(){
    echo -e "\e[33m\e[1m$1"
    echo -e "\e[0m"
}
if [ $# -eq 0 ]; then
    echo "Parameter with Qt directory needed eq. /opt/Qt5.13.1/5.13.1/gcc_64/lib/cmake/Qt5"
    exit 1
fi

clear_dir
build_ci $1

show_yellow_text "Clang tidy starts..."
./run-clang-tidy.py -p build_ci/ -header-filter='genetic-checkers/checkers_*' '^(?=.*genetic-checkers/checkers_)(?!.*tests).*' -quiet

if [ $? -ne 0 ]; then
    show_red_text "Clang tidy failed."
    clear_dir
    exit 1
fi

show_green_text "Clang tidy succeed!"
show_yellow_text "UT tests starts..."
./build_ci/tests/checkers_ut
if [ $? -ne 0 ]; then
    show_red_text "UT tests failed."
    clear_dir
    exit 1
fi
show_green_text "UT tests succeed!"

clear_dir

show_green_text "CI passed, SHIP YOUR CHANGE ASAP!"
exit 0
