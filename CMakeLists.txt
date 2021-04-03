cmake_minimum_required(VERSION 3.12)

project(lfa VERSION 1.0)

add_library(
    automata
    DFA.h
    DFA.cpp
    NFA.h
    NFA.cpp)

add_executable(lfa_exec ${SOURCES} main.cpp)
add_executable(dfa_parser_engine ${SOURCES} ex1.cpp)
add_executable(dfa_acceptance_engine ${SOURCES} ex2.cpp)
add_executable(nfa_acceptance_enginge ${SOURCES} ex3.cpp)
add_executable(transform_nfa_in_dfa_engine ${SOURCES} ex4.cpp)
add_executable(dfa_minimization_engine ${SOURCES} ex5.cpp)

target_link_libraries(lfa_exec PRIVATE automata)
target_link_libraries(dfa_parser_engine PRIVATE automata)
target_link_libraries(dfa_acceptance_engine PRIVATE automata)
target_link_libraries(nfa_acceptance_enginge PRIVATE automata)
target_link_libraries(transform_nfa_in_dfa_engine PRIVATE automata)
target_link_libraries(dfa_minimization_engine PRIVATE automata)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
