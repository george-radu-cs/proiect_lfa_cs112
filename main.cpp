/* pentru teste */

#include "NFA.h"

int main(int argc, char **argv) {
  NFA *nfa = new NFA(argv[1]);

  /* trebuie acceptat */
  if (nfa->isCreated()) {
    std::string cuvant = argv[2];
    bool ok = nfa->acceptaCuvant(cuvant);
    /* trebuie respins */
    cuvant = "aa";
    ok = nfa->acceptaCuvant(cuvant);
  }
  DFA dfa = nfa->transformToDFA();
  dfa.scrieConfig("test.txt");

  /* NFA *nfa2 = new NFA(nfa->sigma, nfa->states, nfa->initial_state, */
  /* nfa->final_states, nfa->transitions); */
  /* if (nfa2->isCreated()) { */
  /* std::string cuvant = argv[2]; */
  /* bool ok = nfa2->acceptaCuvant(cuvant); */
  /* } */

  delete nfa;
  return 0;
}
