<h3 align="center">Genetic checkers</h3>

<p align="center">
  Self-learning software improving checkers play skills using genetic algorithms.
</p>


## Table of contents

- [Dependencies](#dependencies)
- [Quick start](#quick-start)
- [How to use](#how-to-use)
- [How to customize](#how-to-customize)
- [How does it work](#how-does-it-work)
- [Contributing](#contributing)
- [Contact](#contact)

## Dependencies
- Qt5 (tested on 5.13.1)
- g++ with C++17 (tested on 7.3.0)
- cmake (tested on 3.10.2)
- clang-tidy (tested on clang-tidy-9)

## Quick start

- git clone https://github.com/gdomeradzki/genetic-checkers.git
- git submodule init && git submodule update
- mkdir build && cd build
- cmake -DQt5_DIR=/opt/Qt5.13.1/5.13.1/gcc_64/lib/cmake/Qt5 ..
- make all -j$(nproc)

## How to use

There are seven build targets.

Runnable:
- checkers_learning
- checkers_frontend

Libraries
- checkers_ai
- checkers_engine

UT tests:
- checkers_ai_test
- checkers_engine_test
- checkers_learning_test

Checkers_learning is an application responsible for running genetic-algorithm and selecting best specimen which can be loaded  by checkers_fronted application. Other modules are less important

Screenshot from checkers_frontend:
![Image of game board](https://github.com/gdomeradzki/genetic-checkers/blob/master/screenshots/main_window.png)

## How to customize 

The most attractive part for customization is genetic-algoritm. All parameters are available in
```text
genetic-checkers/
└── checkers_learning/
    └── src/
        └──  main.cpp
 ```
 Also minimax searching depth is available to customize in checkers_fronted (available to set from GUI)
 
 ## How does it work
 
 Genetic algorithm uses minimax algorithm and then tournament system to perform selection process. Minimax algorithm evaluates situation on board by set of metrics which in the same time are genes for genetic algorithm.
 
## Contributing

Please feel free to create Pull Request

ToDo list:
 * [ ] Generate and commit demonstrational AI file
 * [ ] Get rid of hardcoded genetic algorithm parameters - make them program parameters
 * [ ] Make one target for all UT tests
 * [ ] Add possibility to load two, different AI in frontend
 * [ ] Do performance improvements (sanitizers etc.)
 * [ ] Check code in terms of memory leaks (by valgrind for example)
 * [ ] Add ut coverage tool
 
 ## Contact
[grzegorz@domeradzki.com](mailto:grzegorz@domeradzki.com)
 
