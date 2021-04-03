/* Implement a library/program in a programming language of your choosing to
 * test acceptance fo an NFA - loaded from an NFA config file (similar to the
 * DFA config file from exercise 1 */

#include "NFA.h"

int main(int argc, char **argv) {
  NFA *nfa = new NFA(argv[1]);

  if (nfa->isCreated()) {
    std::cout << "NFA-ul este valid\n";
    std::string cuvant{argv[2]};
    if (nfa->acceptaCuvant(cuvant)) {
      std::cout << "Cuvantul " << cuvant << " este acceptat de NFA\n";
    } else {
      std::cout << "Cuvantul " << cuvant << " nu este acceptat de NFA\n";
    }
  } else {
    std::cout << "NFA-ul nu este valid\n";
  }

  return 0;
}
