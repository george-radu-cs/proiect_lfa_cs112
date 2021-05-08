/* Implement a library/program in a programming language of your choosing to
 * convert an NFA - loaded from an NFA config file - to a DFA
 * first argument -> the config file for the NFA
 * second argument -> the filename in which we will write the config of the
 * equivalent DFA */

#include "NFA.h"

int main(int argc, char **argv) {
  NFA *nfa = new NFA(argv[1]);

  /* trebuie acceptat */
  if (nfa->isCreated()) {
    DFA dfa = nfa->transformToDFA();
    dfa.scrieConfig(argv[2]);
  }

  delete nfa;

  return 0;
}
