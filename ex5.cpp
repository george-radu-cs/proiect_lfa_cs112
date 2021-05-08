/* Implement a library/program in a programming language of your choosing to
 * convert a DFA to a minimized DFA.A
 * first argument -> the config file for the DFA
 * second argument -> the filename in which we will write the config of the
 * simplified DFA */

#include "DFA.h"

int main(int argc, char **argv) {
  DFA *dfa1 = new DFA(argv[1]);
  if (dfa1->isCreated()) {
    DFA dfa2;
    dfa2 = dfa1->minimizareDFA();
    dfa2.scrieConfig(argv[2]);
  }

  delete dfa1;

  return 0;
}
