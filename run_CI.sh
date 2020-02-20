#!/bin/bash

clear_dir(){
    rm -rf build_ci
}
build_ci(){
    mkdir build_ci
    cd build_ci
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DQt5_DIR=$1 ..
    cd ..
}
show_green_text(){
    echo -e "\e[32m \e[1m $1"
    echo -e "\e[0m"
}
show_red_text(){
    echo -e "\e[31m \e[1m $1"
    echo -e "\e[0m"
}
if [ $# -eq 0 ]; then
    echo "Parameter with Qt directory needed eq. /opt/Qt5.13.1/5.13.1/gcc_64/lib/cmake/Qt5"
    exit 1
fi

clear_dir
build_ci $1

./run-clang-tidy.py -p build_ci/ -header-filter='genetic-checkers/checkers_en*' genetic-checkers/checkers_en* -quiet

if [ $? -ne 0 ]; then
    show_red_text "Clang tidy failed..."
    clear_dir
    exit 1
fi

show_green_text "Clang tidy succeed..."
clear_dir
