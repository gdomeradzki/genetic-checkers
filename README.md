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
- cmake -DQt5_DIR=<your_qt_path> ..
- make all -j$(nproc)

<your_qt_path> example: /opt/Qt5.14.1/5.14.1/gcc_64/lib/cmake/Qt5
## How to use

There are five build targets.

Runnable:
- checkers_learning
- checkers_frontend

Libraries
- checkers_ai
- checkers_engine

UT tests:
- checkers_ut

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
 
Let's begin with alpha-beta pruning algorithm (improvement to classical minimax). To start using it we need to provide an evaluation function which will tell the algorithm how good the current gamestate is.
Evaluation function created for this purpose takes a set of metrics with factors (one factor per one metric) and basing on this collection evaluates the state on game board.
Example of metrics:
- What is the ratio of my pawns to pawns of the enemy?
- Do I have some figures pattern and does my enemy?

Factors are crucial for evaluation process because they have impact on how important is some metric, so then, what values use for them?
This is what the genetic algorithm was created for.
Every genotype is a set of every metric with factors, then for the selection process needs every genotype plays against every other genotype and gets points (3 for win, 1 for draw). And these points says about fitness ratio in genetic algorithm.
Crossbreeding takes two, randomly selected factors and choose any value between them, whereas mutation is choosing any factor for selected metric. Details about this process can be found in source files.

Genetic algorithm uses minimax algorithm and then tournament system to perform selection process. Minimax algorithm evaluates situation on board by set of metrics which in the same time are genes for genetic algorithm.

## Contributing

Please feel free to create Pull Request

ToDo list:
 * [x] Generate and commit demonstrational AI file
 * [ ] Get rid of hardcoded genetic algorithm parameters - make them program parameters
 * [x] Make one target for all UT tests
 * [ ] Add possibility to load two, different AI in frontend
 * [x] Do performance improvements (profilers, clang-tidy etc.)
 * [ ] Check code in terms of memory leaks (valgrind, sanitizers)
 * [ ] Add ut coverage tool

