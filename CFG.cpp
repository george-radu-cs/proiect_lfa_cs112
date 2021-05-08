#include "CFG.h"

CFG::CFG() { created_correctly = true; }

CFG::CFG(std::string file_name) {
  if (read(file_name)) {
    if (validateCFG()) {
      std::cout << "The data is valid, the cfg was created with success\n";
      created_correctly = true;
    } else {
      std::cout << "The data isn't valid, the cfg wasn't created\n";
      created_correctly = false;
    }
  } else {
    created_correctly = false;
  }
}

CFG::~CFG() {}

bool CFG::read(std::string file_name) {
  /* fin will represent the input file for our cfg config */
  std::ifstream fin(file_name);
  if (!fin.is_open()) { /* if the file couldn't be opened */
    std::cout << "The filename does not exists or the program hasn't rights to "
                 "open it\n";
    return false;
  }

  /* check the number of occurrences for each section */
  /* 0 -> occurrences for section nod start
   * 1 -> occurrences for section epsilon
   * 2 -> occurrences for section terminals
   * 3 -> occurrences for section nonterminals
   * 4 -> occurrences for section productions */
  int occurrences[5] = {0, 0, 0, 0, 0};
  /* remember the line number for a better debug & more relevant error codes */
  int line_number{0};
  /* remember the current section where we are in the config */
  std::string curr_section{""};
  /* we read line by line the config file */
  std::string line;
  /* remember if we found errors in the structure of the config */
  bool incorrect_config{false};

  /* while we have lines in the config file and we didn't find any errors */
  while (std::getline(fin, line) && !incorrect_config) {
    /* delete any whitespaces in the current line */
    /* don't delete the whitespaces if we need to read a production because
     * after the start nonterminal  */
    if (curr_section != "Productions") {
      line.erase(std::remove_if(line.begin(), line.end(),
                                [](unsigned char x) { return isspace(x); }),
                 line.end());
    }
    ++line_number;

    /* if the current line is empty or it's a comment */
    if (line[0] == '#' || line.empty()) {
      continue;
    }

    if (line == "End") { /* found the separator End */
      /* check if we can close a section */
      if (curr_section.empty()) {
        std::cout << "On line " << line_number
                  << " we have no section to close\n";
      }
      curr_section = "";

      continue;
    }

    if (line == "Start:") { /* found the section start node */
      /* we can't have nested sections */
      if (!curr_section.empty()) {
        std::cout << "On line " << line_number
                  << " is the section Start defined inside the section "
                  << curr_section << "\n";
        incorrect_config = true;
      }

      /* the config file must contain only a section Start */
      if (occurrences[0] != 0) {
        std::cout
            << "The config file contains multiple sections of type Start\n";
      }

      occurrences[0]++;
      curr_section = "Start";

      continue;
    }

    if (line == "Epsilon:") { /* found the section epsilon */
      /* we can't have nested sections */
      if (!curr_section.empty()) {
        std::cout << "On line " << line_number
                  << " is the section Epsilon defined inside the section "
                  << curr_section << "\n";
        incorrect_config = true;
      }

      /* the config file must contain only a section Epsilon */
      if (occurrences[1] != 0) {
        std::cout
            << "The config file contains multiple sections of type Epsilon\n";
      }

      occurrences[1]++;
      curr_section = "Epsilon";

      continue;
    }

    if (line == "Terminals:") { /* found the section terminals */
      /* we can't have nested sections */
      if (!curr_section.empty()) {
        std::cout << "On line " << line_number
                  << " is the section Terminals defined inside the section "
                  << curr_section << "\n";
        incorrect_config = true;
      }

      /* the config file must contain only a section Terminals */
      if (occurrences[2] != 0) {
        std::cout
            << "The config file contains multiple sections of type Terminals\n";
      }

      occurrences[2]++;
      curr_section = "Terminals";

      continue;
    }

    if (line == "Nonterminals:") { /* found the section Nonterminals */
      /* we can't have nested sections */
      if (!curr_section.empty()) {
        std::cout << "On line " << line_number
                  << " is the section Nonterminals defined inside the section "
                  << curr_section << "\n";
        incorrect_config = true;
      }

      /* the config file must contain only a section Nonterminals */
      if (occurrences[3] != 0) {
        std::cout << "The config file contains multiple sections of type "
                     "Nonterminals\n";
      }

      occurrences[3]++;
      curr_section = "Nonterminals";

      continue;
    }

    if (line == "Productions:") { /* fount the section Productions */
      /* we can't have nested sections */
      if (!curr_section.empty()) {
        std::cout << "On line " << line_number
                  << " is the section Productions defined inside the section "
                  << curr_section << "\n";
        incorrect_config = true;
      }

      /* the config file must contain only a section Productions */
      if (occurrences[4] != 0) {
        std::cout << "The config file contains multiple sections of type "
                     "Productions\n";
      }

      occurrences[4]++;
      curr_section = "Productions";

      continue;
    }

    if (curr_section == "Start") { /* we are in the section Start node */
      /* the start node must have only a char */
      if (line.length() != 1) {
        std::cout << "The start node must have only a char\n";
        incorrect_config = true;
      }
      start_node = line[0];
    }

    if (curr_section == "Epsilon") { /* we are in the section Epsilon */
      /* epsilon must have only a char */
      if (line.length() != 1) {
        std::cout << "Epsilon must have only a char\n";
        incorrect_config = true;
      }
      epsilon = line[0];
    }

    if (curr_section == "Terminals") { /* we are in the section Terminals */
      /* any terminal must have only a char */
      if (line.length() != 1) {
        std::cout << "Any terminal must have only a char\n";
        incorrect_config = true;
      }
      terminals.insert(line[0]);
    }

    /* we are int the section Nonterminals */
    if (curr_section == "Nonterminals") {
      /* any nonterminal must have only a char */
      if (line.length() != 1) {
        std::cout << "Any nonterminal must have only a char\n";
        incorrect_config = true;
      }
      nonterminals.insert(line[0]);
    }

    /* we are in the section Productions */
    if (curr_section == "Productions") {
      /* the current line contains a space and maybe '|', we will replace each
       * '|' with ' ' and transform the string which contains the current line
       * in an input string stream from which we will read the rest of the
       * informations */
      std::replace(line.begin(), line.end(), '|', ' ');
      std::istringstream iss_line(line);

      /* we read the the starting node of the production, we define it as a
       * string because we don't know if in the config we receive only a char,
       * we need to check that */
      std::string t_str;
      iss_line >> t_str;

      char t_c; /* remember the char of the start nonterminal in the prod */
      if (t_str.length() != 1) {
        incorrect_config = true;
        std::cout << "The start node in a production is a noterminal and must "
                     "have only 1 char";
      } else {
        t_c = t_str[0];
      }

      /* remember in a set all the productions that start with t_c */
      std::set<std::string> tps;
      /* we will read in the string prod each production */
      std::string prod;
      while (iss_line >> prod) {
        tps.insert(prod);
      }

      if (productions.find(t_c) != productions.end()) {
        std::cout
            << "The config should contain all the productions that stats with "
            << t_c << " on a single line\n";
        incorrect_config = true;
        continue;
      }

      /* if the set of productions is empty then the config is invalid, we
       * should receive at least one string after the starting node */
      if (tps.empty()) {
        std::cout
            << "The production should contain at least one string with "
               "terminals and/or nonterminals after the starting nonterminal\n";
        incorrect_config = true;
        continue;
      }

      productions.insert(std::make_pair(t_c, tps));
    }
  }

  fin.close(); /* close the file after reading */

  if (incorrect_config) { /* if we found errors in the config */
    return false;
  }

  for (int i = 0; i < 5; i++) {
    if (occurrences[i] != 1) {
      switch (i) {
      case 0:
        std::cout << "Start section doesn't exists\n";
        break;

      case 1:
        std::cout << "Epsilon section doesn't exists\n";
        break;

      case 2:
        std::cout << "Terminals section doesn't exists\n";
        break;

      case 3:
        std::cout << "Nonterminals section doesn't exists\n";
        break;

      case 4:
        std::cout << "Productions section doesn't exists\n";
        break;

      default:
        break;
      }

      /* if at leas one section is missing then the config is invalid */
      return false;
    }
  }

  /* if we read all the config file and we didn't find any error then the
   * structure of the config until now is valid so we return true, we need to
   * validate it now */
  return true;
}

bool CFG::validateCFG() {
  /* check that the start nonterminal is uppercase */
  if (start_node < 'A' || start_node > 'Z') {
    std::cout
        << "The start nonterminal must be uppercase, The CFG is invalid\n";
    created_correctly = false;
    return false;
  }

  /* check that all the terminals are lowercase letters */
  for (const auto &c : terminals) {
    if (c < 'a' || c > 'z') {
      std::cout << c << "Any terminal must be uppercase, The CFG is invalid\n";
      created_correctly = false;
      return false;
    }
  }

  /* check that all the nonterminals are uppercase letters */
  for (const auto &c : nonterminals) {
    if (c < 'A' || c > 'Z') {
      std::cout << c
                << "Any nonterminal must be uppercase, The CFG is invalid\n";
      created_correctly = false;
      return false;
    }
  }

  /* check if all the productions are valid, we consider a production valid if
   * it's start nonterminal is in the set of nonterminals or is the start_node,
   * and all the other string from the production leads to
   * terminals/nonterminals/start_node/epsilon, other values which aren't known
   * by the cfg are invalid */
  for (const auto &p : productions) {
    /* check the departure nonterminal */
    if (!(nonterminals.find(p.first) != nonterminals.end() ||
          p.first == start_node)) {
      std::cout << "All the productions that start with " << p.first
                << " are wrong because " << p.first
                << " is not in the set of nonterminals/variables\n";
      created_correctly = false;
      return false;
    }

    /* check all the productions starting from p.first */
    for (const auto &ps : p.second) {
      /* check each terminal and nonterminal if it's in their sets known by the
       * cfg */
      for (const auto &c : ps) { /* each terminal and nonterminal is a char */
        /* check if we verify a terminal or a nonterminal */
        if ('a' <= c && c <= 'z') {
          if (terminals.find(c) == terminals.end()) {
            std::cout << c
                      << " isn't in the set of terminals, but it appears in a "
                         "production, so the production is wrong, thus the cfg "
                         "is invalid\n";
            created_correctly = false;
            return false;
          }
        } else if ('A' <= c && c <= 'Z') {
          if (nonterminals.find(c) == nonterminals.end()) {
            std::cout << c
                      << " isn't in the set of nonterminals, but it appears in "
                         "a production, so the production is wrong, thus the "
                         "cfg is invalid\n";
            created_correctly = false;
            return false;
          }
        } else if (c != epsilon) { /* now c could be only an epsilon */
          std::cout << c
                    << " is neither a terminal, nor a nonterminal not even the "
                       "epsilon, it's a value unknow for the cfg, so the cfg "
                       "is invalid\n";
          created_correctly = false;
          return false;
        } else if (c == epsilon && ps.length() != 1) {
          std::cout << "In a production epsilon shouldn't be in the same "
                       "string with other terminal or nonterminals, we found "
                       "this case, thus the cfg is invalid\n";
          created_correctly = false;
          return false;
        }
      }
    }
  }

  return true;
}

void CFG::removeNullProductions() {
  /* first identify the null productions, a null production goes from a
   * nonterminal in epsilon */
  for (const auto &production : productions) { /* search for null productions */
    for (const auto &production_right : production.second) {
      if (production_right[0] == epsilon) { /* found a null production */
        char nt = production.first; /* remember the nonterminal for which we
                                 found a null production that starts with it */
        /* delete the null production */
        productions[nt].erase(&epsilon);

        /* now for each production that contains nt transform in its right side
         * string add create productions in which we replace each occurrences of
         * nt in each of these productions with epsilon(in our case with empty
         * space) */
        for (const auto &prod : productions) {
          /* prod_right represent the right part of every production that starts
           * with prod.first, we will use the same notation later */
          for (const auto &prod_right : prod.second) {
            /* found nt in a production */
            if (prod_right.find(nt) != std::string::npos) {
              /* create all the possible variations by replacing each time the
               * char nt with the empty string in the current production and add
               * them to the production set that starts with prod.first; if we
               * have a string that contains multiples nt, after we finish it we
               * will find the new productions with one less nt and we will
               * process them again until we reach the production the substring
               * of the original string with 0 occurrences of nt */
              size_t position = prod_right.find(nt);
              while (position != std::string::npos) {
                /* find the next occurrence of nt */
                std::string new_production = prod_right.substr(0, position) +
                                             prod_right.substr(position + 1);

                /* delete the occurrence of nt */
                position = prod_right.find(nt, position + 1);

                /* add the new production */
                productions[prod.first].insert(new_production);
              }
            }
          }
        }

        break;
      }
    }
  }
}

void CFG::removeUnitProductions() {
  /* remember if we found an unit production in the previous iteration, if we
   * didn't then we stop the loop */
  bool found_unit_prod;
  std::set<std::pair<char, std::string>> unit_productions_found;
  do {
    found_unit_prod = false; /* reset the checker */

    /* search unit productions */
    for (const auto &production : productions) {
      for (const auto &production_right : production.second) {
        if (production_right.length() == 1 &&
            terminals.find(production_right[0]) == terminals.end() &&
            unit_productions_found.find(
                std::make_pair(production.first, production_right)) ==
                unit_productions_found.end()) {

          /* mark that we find at least one unit production */
          found_unit_prod = true;

          /* save the unit prodution to delete after, if we don't find a
           * prodution of type production_right->terminal then this production
           * is an useless production and it doesn't matter if delete it */
          unit_productions_found.insert(
              std::make_pair(production.first, production_right));

          /* if we found an unit production that lead to a terminal then
           * create a new production of type production.first->terminal */
          if (productions.find(production_right[0]) != productions.end()) {
            for (const auto &prod_right : productions[production_right[0]]) {
              if (prod_right.length() == 1 &&
                  terminals.find(prod_right[0]) != terminals.end()) {
                productions[production.first].insert(prod_right);
              }
            }
          }
        }
      }
    }

  } while (found_unit_prod);

  /* remove the unit productions found previously */
  for (const auto &p : unit_productions_found) {
    productions[p.first].erase(p.second);
  }

  /* if we delete all the productions for a nonterminal in the previous step */
  for (const auto &p : unit_productions_found) {
    if(productions[p.first].empty()){
      productions.erase(p.first);
    } 
  }
}

void CFG::removeUselessProductions() {
  /* first step: delete the nonterminals that blocks the word(i.e. the cfg
   * doesn't have any productions that starts with those nonterminals) */

  std::set<char> new_nonterminals;
  /* at the first iteration add all the nonterminals that cam derive the
   * terminals(i.e. the production that starts with the nonterminal has a
   * terminal in his right part)*/
  for (const auto &prod : productions) {
    for (const auto &prod_right : prod.second) {
      for (const auto &c : prod_right) {
        if (terminals.find(c) != terminals.end()) {
          new_nonterminals.insert(prod.first);
        }
      }
    }
  }

  bool added_new_terminal{true};
  /* iterate until we can't add more new nonterminals */
  /* size_t nr_iteration{1}; */
  while (added_new_terminal) {
    /* /1* for testing *1/ */
    /* std::cout << nr_iteration << ":"; */
    /* for (const auto &c : new_nonterminals) { */
    /*   std::cout << c << " "; */
    /* } */
    /* std::cout << '\n'; */
    /* ++nr_iteration; */
    /* /1* finish testing *1/ */

    added_new_terminal = false;

    for (const auto &prod : productions) { /* search through each production */
      /* if we don't have already the nonterminal that it's the left part of
       * this production */
      if (new_nonterminals.find(prod.first) == new_nonterminals.end()) {
        for (const auto &prod_right : prod.second) {
          /* for each nonterminal in the production */
          for (const auto &c : prod_right) {
            if (nonterminals.find(c) != nonterminals.end() &&
                new_nonterminals.find(c) != new_nonterminals.end()) {
              new_nonterminals.insert(prod.first);
              added_new_terminal = true;
            }
          }
        }
      }
    }
  }

  /* delete all the nonterminals that aren't in the new_nonterminals set and
   * the production in which they appear  */
  /* remember which nonterminals we delete */
  std::set<char> deleted_nonterminals;
  /* delete the useless nonterminals from the nonterminals set of cfg */
  for (const auto &nt : nonterminals) {
    if (new_nonterminals.find(nt) == new_nonterminals.end()) {
      deleted_nonterminals.insert(nt);
    }
  }
  for (const auto &nt : deleted_nonterminals) {
    nonterminals.erase(nt);
  }
  
  /* find which productions contains useless nonterminals */
  std::set<std::pair<char, std::string>> productions_to_delete;
  for (const auto &prod : productions) {
    for (const auto &prod_right : prod.second) {
      for (const auto &c : prod_right) {
        if (deleted_nonterminals.find(c) != deleted_nonterminals.end()) {
          /* productions[prod.first].erase(prod_right); */
          productions_to_delete.insert(std::make_pair(prod.first, prod_right));
        }
      }
    }
  }
  /* delete the productions which contains the useless nonterminals */
  for (const auto &p : productions_to_delete) {
    productions[p.first].erase(p.second);
  }

  /* second step: delete all the terminals and nonterminals that we cannot reach
   * and the productions that starts from those nonterminals */

  /* remember the final terminals and nonterminals */
  std::set<char> final_set_nonterminals;
  std::set<char> final_set_terminals;

  final_set_nonterminals.insert(start_node);
  /* if we added a terminal or a nonterminal on the previous iteration then try
   * a new iteration until we cannot add anything new */
  bool need_new_iteration{true};
  while (need_new_iteration) {
    need_new_iteration = false;

    for (const auto &production : productions) {
      if (final_set_nonterminals.find(production.first) !=
          final_set_nonterminals.end()) {
        for (const auto &production_right : production.second) {
          for (const auto &c : production_right) {
            if ('a' <= c && c <= 'z') { /* c is a terminal */
              if (final_set_terminals.find(c) == final_set_terminals.end()) {
                final_set_terminals.insert(c);
                need_new_iteration = true;
              }
            } else { /* c is a nonterminal */
              if (final_set_nonterminals.find(c) ==
                  final_set_nonterminals.end()) {
                final_set_nonterminals.insert(c);
                need_new_iteration = true;
              }
            }
          }
        }
      }
    }
  }

  /* now delete the remaining terminals */
  std::set<char> terminals_to_delete;
  for (const auto &t : terminals) {
    if (final_set_terminals.find(t) == final_set_terminals.end()) {
      terminals_to_delete.insert(t);
    }
  }

  for (const auto &t : terminals_to_delete) {
    terminals.erase(t);
  }

  /* now delete the remaining nonterminals */
  std::set<char> nonterminals_to_delete;
  for (const auto &nt : nonterminals) {
    if (final_set_nonterminals.find(nt) == final_set_nonterminals.end()) {
      nonterminals_to_delete.insert(nt);
    }
  }

  for (const auto &nt : nonterminals_to_delete) {
    nonterminals.erase(nt);
  }

  /* and finally delete all productions that starts with an useless terminal
   * which we deleted */
  for (const auto &nt : nonterminals_to_delete) {
    productions.erase(nt);
  }
}

void CFG::simplifyCFG() {
  /* 1) remove null productions */
  removeNullProductions();
  /* 2) remove unit productions */
  removeUnitProductions();
  /* 3) remove useless productions */
  removeUselessProductions();
}

std::ostream &operator<<(std::ostream &out, const CFG &cfg) {
  out << cfg.start_node << "\n" << cfg.epsilon << "\n{";

  /* print the non terminals */
  for (const auto &v : cfg.nonterminals) {
    out << v << ", ";
  }

  /* we use \b\b to delete the comma and the space after the last nonterminal */
  out << "\b\b} -> non terminals\n{";

  /* print the terminals */
  for (const auto &t : cfg.terminals) {
    out << t << ", ";
  }

  /* we use \b\b to delete the comma and the space after the last terminal */
  out << "\b\b} -> terminals\n\n";

  /* print the productions */
  for (const auto &p : cfg.productions) {
    out << p.first << " -> ";
    for (const auto &pv : p.second) {
      out << pv << "|";
    }
    out << "\b \n";
  }

  return out;
}
