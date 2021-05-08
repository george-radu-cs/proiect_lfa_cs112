/* Implement a library/program in a programming  language of your choosing to
 * test acceptance of a DFA - loaded from a DFA config file. */

#include "DFA.h"

int main(int argc, char **argv) {
  DFA *dfa = new DFA(argv[1]);

  if (dfa->isCreated()) {
    std::string cuvant = argv[2];
    if (dfa->acceptaCuvant(cuvant)) {
      std::cout << "Cuvantul " << cuvant << " este acceptat de DFA\n";
    } else {
      std::cout << "Cuvantul " << cuvant << " nu este acceptat de DFA\n";
    }
  }

  delete dfa;

  return 0;
}
