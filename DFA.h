#ifndef DFA_H
#define DFA_H

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>

class DFA {
protected:
  std::vector<char> sigma; /* alfabetul */
  std::vector<int> states; /* setul de stari */
  int initial_state;
  std::vector<int> final_states; /* setul de stari finale */
  /* functia de tranzitie */
  std::vector<std::tuple<int, char, int>> transitions;

  bool created_correctly; /* retinem daca dfa ul este unul valid */

  /* validam corectitudinea definirii dfa ului */
  virtual bool validareDate();

  /* citim dfa ul dintr un fisier si facem verificari semantice */
  virtual bool citireValidare(std::string file_name);

public:
  DFA() {} /* constructor fara parametrii */

  DFA(std::string file_name); /* constructor cu param nume fisier config */

  DFA(const std::vector<char> &sigma, const std::vector<int> &states,
      const int is, const std::vector<int> &final_states,
      const std::vector<std::tuple<int, char, int>> &transitions);

 virtual ~DFA() {} /* destructor */

  /* verificam daca DFA-ul a fost creat corect */
  bool isCreated() const { return created_correctly; }

  /* metoda pentru a verifica daca dfa ul accepta un cuvant dat */
  bool acceptaCuvant(std::string cuvant);

  /* minimalizeaza un DFA complet */
  DFA &minimizareDFA();

  /* scriem configuratia DFA ului intr-un fisier */
  void scrieConfig(std::string file_name);
};

#endif
