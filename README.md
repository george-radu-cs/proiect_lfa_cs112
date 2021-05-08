# Proiect LFA

## Compilare

Se va construi libraria _automata_ care contine clasele DFA si NFA si se vor
crea executabile pentru fiecare cerinta in parte + un executabil lfa_exec care
l-am folosit pentru teste, se poate ignora acesta.

> $ cmake CMakeLists.txt && make

## Ex. 1

Pentru a testa un DFA valid:

> $ ./dfa_parser_engine config_dfa.txt

Pentru a teste un DFA invalid. (vom folosi un config de nfa):

> $ ./dfa_parser_engine config_nfa.txt

## Ex. 2

Pentru a testa ca un DFA accepta un cuvant:

> $ ./dfa_acceptance_engine config_dfa.txt ab

Pentru a testa ca un DFA nu accepta un cuvant:

> $ ./dfa_acceptance_engine config_dfa.txt a

## Ex. 3

Pentru a testa ca un NFA accepta un cuvant:

> $ ./nfa_acceptance_enginge config_nfa.txt ab

Pentru a testa ca un NFA nu accepta un cuvant:

> $ ./nfa_acceptance_enginge config_nfa.txt a

## Ex. 4

Convertim un NFA intr-un DFA:

> $ ./transform_nfa_in_dfa_engine 4_config_nfa.txt 4_config_dfa.txt

## Ex. 5

Minimizam un DFA:

> $ ./dfa_minimization_engine 5_config_dfa.txt 5_config_dfa_minimized.txt

## Ex. 6

Simplificarea unui CFG:

> $ ./cfg_simplifier_engine 6_config_cfg.txt 6_config_cfg_simplified.txt

> $ ./cfg_simplifier_engine 6_1_cfg.txt 6_config_cfg_simplified.txt

> $ ./cfg_simplifier_engine 6_2_cfg.txt 6_config_cfg_simplified.txt

> $ ./cfg_simplifier_engine 6_3_cfg.txt 6_config_cfg_simplified.txt

## TODO

- turing machine
- translate all the project
