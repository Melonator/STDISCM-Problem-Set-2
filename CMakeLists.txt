cmake_minimum_required(VERSION 3.30)
project(STDISCM_ProblemSet_2)

set(CMAKE_CXX_STANDARD 20)

add_executable(STDISCM_ProblemSet_2 main.cpp
        implementation/Graph.cpp
        implementation/Pathfinder.cpp
        implementation/ThreadPool.cpp
        implementation/sequential/AStarSequential.cpp
        implementation/sequential/BFSSequential.cpp
        implementation/sequential/PrimePathSequential.cpp
        implementation/sequential/ShortestPrimePathSequential.cpp
        implementation/threaded/AStarThreaded.cpp
        implementation/threaded/BFSThreaded.cpp
        implementation/threaded/PrimePathThreaded.cpp
        implementation/threaded/ShortestPrimePathThreaded.cpp
        implementation/IPathBuilder.cpp)
