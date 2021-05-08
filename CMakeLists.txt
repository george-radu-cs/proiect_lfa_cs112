cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

project(lfa VERSION 1.0)

add_library(
    automata
    DFA.h
    DFA.cpp
    NFA.h
    NFA.cpp)

add_library(
    cfg
    CFG.h
    CFG.cpp)

add_executable(lfa_exec  main.cpp)
add_executable(dfa_parser_engine  ex1.cpp)
add_executable(dfa_acceptance_engine  ex2.cpp)
add_executable(nfa_acceptance_enginge  ex3.cpp)
add_executable(transform_nfa_in_dfa_engine  ex4.cpp)
add_executable(dfa_minimization_engine  ex5.cpp)
add_executable(cfg_simplifier_engine ex6.cpp)

target_link_libraries(lfa_exec PRIVATE automata)
target_link_libraries(dfa_parser_engine PRIVATE automata)
target_link_libraries(dfa_acceptance_engine PRIVATE automata)
target_link_libraries(nfa_acceptance_enginge PRIVATE automata)
target_link_libraries(transform_nfa_in_dfa_engine PRIVATE automata)
target_link_libraries(dfa_minimization_engine PRIVATE automata)
target_link_libraries(cfg_simplifier_engine PRIVATE cfg)
