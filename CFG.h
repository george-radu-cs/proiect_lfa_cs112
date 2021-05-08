#ifndef CFG_H
#define CFG_H

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>

class CFG {
private:
  char start_node; /* start variable/node/nonterminal */
  char epsilon;    /* what character will represent the epsilon in our config */
  /* set of terminals, we note them with lowercase letter */
  std::set<char> terminals;
  /* set of nonterminals/variables, we note them with capital letter */
  std::set<char> nonterminals;
  /* productions/rules for cfg */
  std::unordered_map<char, std::set<std::string>> productions;

  bool created_correctly; /* remember if the cfg is valid */

  /* read the cfg from its config file and make some validation about the config
   * file structure */
  bool read(std::string file_name);

  /* validate the correctness of the cfg */
  bool validateCFG();

  /* first identify the null productions, a null production goes from a
   * nonterminal in epsilon */
  void removeNullProductions();

  /* remove units that are tranzitive and could be simplified from 3
   * nonterminals in 2 or from 2 nonterminals and 1 terminal in 1 nonterminals
   * and 1 terminal */
  void removeUnitProductions();

  /* remove the productions we cannot reach from the start node and the
   * productions which block a word */
  void removeUselessProductions();

public:
  CFG();

  /* constructor which receive the filename of the config */
  CFG(std::string file_name);

  ~CFG(); /* destructor */

  bool isCreated() const { return created_correctly; }

  /* simplify a CFG */
  void simplifyCFG();

  /* show the configuration of the CFG to an output stream */
  friend std::ostream &operator<<(std::ostream &out, const CFG &cfg);
};

#endif
