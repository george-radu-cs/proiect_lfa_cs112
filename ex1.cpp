/* Implement a library/program in a programming language of your choosing to
 * load and validate a DFA input file */

#include "DFA.h"

int main(int argc, char **argv) {
  DFA *dfa = new DFA(argv[1]);

  if (dfa->isCreated()) {
    std::cout << "DFA-ul este valid\n";
  } else {
    std::cout << "DFA-ul nu este valid\n";
  }

  delete dfa;

  return 0;
}
