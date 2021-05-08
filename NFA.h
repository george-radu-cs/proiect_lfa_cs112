#ifndef NFA_H
#define NFA_H

#include "DFA.h"

class NFA : public DFA {
private:
  /* std::vector<char> sigma; /1* alfabetul *1/ */
  /* std::vector<int> states; /1* setul de stari *1/ */
  /* int initial_state; */
  /* std::vector<int> final_states; /1* setul de stari finale *1/ */
  /* functia de tranzitie */
  /* std::vector<std::tuple<int, char, int>> transitions; */

  /* bool created_correctly; /1* retinem daca nfa ul este unul valid *1/ */

  /* validam corectitudinea definirii nfa ului */
  bool validareDate() override;

  /* citim nfa ul dintr un fisier si facem verificari semantice */
  bool citireValidare(std::string file_name) override;

public:
  NFA() : DFA() {} /* constructor fara parametrii */

  NFA(std::string file_name); /* constructor cu param nume fisier config */

 virtual ~NFA() {} /* destructor */

  /* verificam daca NFA-ul a fost creat corect */
  bool isCreated() const { return created_correctly; }

  /* metoda pentru a verifica daca nfa ul accepta un cuvant dat
   * trimitem cuvantul si starea curenta, pentru apelul functiei din exteriorul
   * clasei nu trebuie mentionata starea curenta (care va fi cea initiala),
   * aceasta va fi setata la primul call recursiv al metodei, consideram ca nu
   * vom avea in setul de stari starea -1 */
  bool acceptaCuvant(std::string cuvant, int cs = -1);

  /* scrie configul nfa ului curent */
  /* void scrieConfig(std::string file_name); */

  /* transforma NFA curent intr-un DFA */
  DFA &transformToDFA();
};

#endif
