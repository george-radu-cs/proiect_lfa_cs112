#include "NFA.h"

NFA::NFA(std::string file_name) : DFA() {
  if (citireValidare(file_name)) {
    if (validareDate()) {
      std::cout << "Datele sunt valide, a fost creat NFA-ul\n";
      created_correctly = true;
    } else {
      std::cout << "Datele nu sunt valide, nu a fost creat NFA-ul\n";
      created_correctly = false;
    }
  } else {
    std::cout << "nu a fost creat NFA-ul\n";
    created_correctly = false;
  }
}

bool NFA::validareDate() {
  /* verificam sa nu avem duplicate in alfabet */
  unsigned sigma_len = sigma.size();
  for (unsigned i = 0; i < sigma_len - 1; i++)
    for (unsigned j = i + 1; j < sigma_len; j++) {
      if (sigma.at(i) == sigma.at(j)) {
        std::cout << "Duplicat in alfabet: " << sigma.at(i) << "\n";
        return false;
      }
    }

  /* verificam sa nu avem duplicate in setul de stari */
  unsigned states_len = states.size();
  for (unsigned i = 0; i < states_len - 1; i++)
    for (unsigned j = i + 1; j < states_len; j++) {
      if (states.at(i) == states.at(j)) {
        std::cout << "Duplicat in stari: " << states.at(i) << "\n";
        return false;
      }
    }

  /* verificam sa nu avem duplicate in functia de tranzitie */
  unsigned transitions_len = transitions.size();
  for (unsigned i = 0; i < transitions_len - 1; i++)
    for (unsigned j = i + 1; j < transitions_len; j++) {
      if (transitions.at(i) == transitions.at(j)) {
        std::cout << "Duplicat in tranzitii\n";
        return false;
      }
    }

  for (unsigned i = 0; i < transitions_len; i++) {
    /* vector de frecventa stare1,litera,stare2 */
    bool fr[3] = {false};

    /* obtinem detaliile despre tranzitia curenta */
    int s1 = std::get<0>(transitions.at(i));
    int s2 = std::get<2>(transitions.at(i));
    char litera = std::get<1>(transitions.at(i));

    /* verificam ca litera din tranzitie sa fie in sigma */
    for (unsigned j = 0; j < sigma_len; j++) {
      if (sigma.at(j) == litera) {
        fr[1] = true;
        break;
      }
    }

    /* verificam ca starile din tranzitie sa fie in setul de stari care definesc
     * DFA ul */
    for (unsigned j = 0; j < states_len; j++) {
      if (s1 == states.at(j))
        fr[0] = true;
      if (s2 == states.at(j))
        fr[2] = true;
    }

    for (int k = 0; k < 3; k++)
      if (!fr[k]) {
        std::cout << "Eroare la tranzitia " << i << " " << k << "\n";
        return false;
      }
  }

  return true;
}

bool NFA::citireValidare(std::string file_name) {
  std::ifstream fin(file_name);
  if (!fin.is_open()) { /* daca nu am putut deschide fisierul config */
    std::cout << "fisierul nu exista sau nu avem permisiune de citire\n";
    return false;
  }

  int fr_si = 0; /* nr aparitii stare initiala */
  /* delimitare[0] nr aparitii pt Sigma
   * delimitare[1] nr aparitii pt States
   * delimitare[2] nr aparitii pt Transitions */
  int delimitare[3] = {0, 0, 0};
  int line_number{1};
  std::string curr_section{""};
  std::string line;
  bool config_incorect{false};

  /* cat timp mai sunt linii in config si nu am descoperit ca avem erori in
   * config de orice tip  */
  while (std::getline(fin, line) && !config_incorect) {
    /* stergem toate spatiile din linia curenta */
    line.erase(std::remove_if(line.begin(), line.end(),
                              [](unsigned char x) { return std::isspace(x); }),
               line.end());
    /* daca linia curenta este goala sau este un comentariu */
    if (line[0] == '#' || line.empty())
      continue;

    if (line == "End") { /* daca am intalnit separatorul end */
      /* verificam ca avem ce sectiune sa inchidem */
      if (curr_section.empty()) {
        std::cout << "Pe linia " << line_number
                  << " nu avem ce sectiune sa inchidem\n";
        config_incorect = true;
      }
      curr_section = "";
      continue;
    }

    if (line == "Sigma:") { /* daca am gasit inceputul sectiunii sigma */
      /* nu putem avea definita o sectiune in alta sectiune */
      if (!curr_section.empty()) {
        std::cout
            << "Pe linia " << line_number
            << " se afla sectiunea sigma definita in interiorul sectiunii "
            << curr_section << "\n";
        config_incorect = true;
      }

      if (delimitare[0] != 0) { /* nu putem avea mai multe definitii sigma */
        std::cout << "Exista mai multe definitii de sectiuni de sigma\n";
        config_incorect = true;
      }

      delimitare[0] = 1;
      curr_section = "Sigma";
      continue;
    }

    if (line == "States:") { /* daca am gasit inceputul sectiunii states */
      /* nu putem avea definita o sectiune in alta sectiune */
      if (!curr_section.empty()) {
        std::cout
            << "Pe linia " << line_number
            << " se afla sectiunea stari definita in interiorul sectiunii "
            << curr_section << "\n";
        config_incorect = true;
      }

      if (delimitare[1] != 0) { /* nu putem avea mai multe definitii states */
        std::cout << "Exista mai multe definitii de sectiuni de stari\n";
        config_incorect = true;
      }

      delimitare[1] = 1;
      curr_section = "States";
      continue;
    }

    /* daca am gasit inceputul sectiunii transitions */
    if (line == "Transitions:") {
      /* nu putem avea definita o sectiune in alta sectiune */
      if (!curr_section.empty()) {
        std::cout
            << "Pe linia " << line_number
            << " se afla sectiunea tranzitii definita in interiorul sectiunii "
            << curr_section << "\n";
        config_incorect = true;
      }
      /* nu putem avea mai multe definitii transitions */
      if (delimitare[2] != 0) {
        std::cout << "Exista mai multe definitii de sectiuni de tranzitii\n";
        config_incorect = true;
      }

      delimitare[2] = 1;
      curr_section = "Transitions";
      continue;
    }

    if (curr_section == "Sigma") { /* daca suntem in sectiunea sigma */
      /* am impus conditia ca literele din alfabet sa aiba un singur caracter */
      if (line.length() != 1) {
        std::cout
            << "Literele din alfabet trebuie sa aiba un singur caracter\n";
        config_incorect = true;
      }
      sigma.push_back(line[0]); /* adaugam litera in alfabet */
    }

    if (curr_section == "States") { /* daca suntem in sectiunea states */
      /* cautam separatorul virgula, in caz ca aceasta lipseste starea curenta
       * nu este nici initiala nici finala */
      size_t position = line.find(',');

      /* daca linia curenta nu este stare finala sau initiala */
      if (position == std::string::npos) {
        try {
          int val = std::stoi(line);
          states.push_back(val);
        } catch (std::invalid_argument) {
          std::cout << "Pe linia " << line_number << " nu se afla un numar\n";
          config_incorect = true;
        }
      } else {
        /* putem avea dupa virgula la state atat S cat si F sau nimic
         * OBS: acceptam doar 1 1,S 1,F 1,SF nu si 1,FS */
        if (line.substr(position + 1).length() > 2) {
          std::cout << line << "\n" << line.substr(position + 1) << '\n';
          std::cout << "Pe linia " << line_number
                    << " avem prea multe caractere dupa virgula\n";
          config_incorect = true;
        }
        /* daca starea curenta este initiala sau finala */
        if (line.substr(position + 1).length() == 1) {
          /* daca starea curenta este initiala */
          if (line[position + 1] == 'S' || line[position + 1] == 's') {
            if (fr_si != 0) { /* nu putem avea decat o stare initiala, daca am
                                 gasit mai devreme o alta stare initiala, atunci
                                 configul dfa ului este gresit */
              std::cout << "Pe linia " << line_number
                        << " este definita inca o stare initiala\n";
              config_incorect = true;
            }
            ++fr_si;
            initial_state = std::stoi(line.substr(0, position));
            states.push_back(initial_state);
          } else if (line[position + 1] == 'F' || line[position + 1] == 'f') {
            /* daca starea curenta este finala */
            final_states.push_back(std::stoi(line.substr(0, position)));
            states.push_back(std::stoi(line.substr(0, position)));
          } else {
            std::cout
                << "Pe linia " << line_number
                << " este definita o stare diferita de finala sau initiala\n";
          }
        } else { /* starea curenta trebuie sa fie atat initiala cat si finala */
          if (line[position + 1] == 'S' || line[position + 1] == 's') {
            if (fr_si != 0) {
              std::cout << "Pe linia " << line_number
                        << " este definita inca o stare initiala\n";
              config_incorect = true;
            }
            ++fr_si;
            initial_state = std::stoi(line.substr(0, position));
          } else {
            std::cout << "Pe linia " << line_number
                      << " este o eroare in definirea statetului\n";
            config_incorect = true;
          }
          if (line[position + 2] == 'F' || line[position + 2] == 'f') {
            final_states.push_back(std::stoi(line.substr(0, position)));

            states.push_back(std::stoi(line.substr(0, position)));
          } else {
            std::cout << "Pe linia " << line_number
                      << " este o eroare in definirea statetului\n";
            config_incorect = true;
          }
        }
      }
    }

    /* daca suntem in sectiunea transitions */
    if (curr_section == "Transitions") {
      /* o tranzitie este separata prin 2 virgule
       * prima virgula separa starea din care plecam si litera cu care plecam
       * iar a doua virgula separa litera cu care plecam si starea in care
       * ajungem, in linia curenta trebuie sa avem 2 virgule */

      size_t p_first = line.find(','); /* cautam prima virgula */

      /* verificam daca am gasit prima virgula */
      if (p_first == std::string::npos) {
        std::cout << "Pe linia " << line_number
                  << " se afla o tranzitie invalida\n";
      } else { /* am gasit prima virgula */
        /* cautam a doua virgula in substr dupa prima virgula */
        size_t p_second = line.substr(p_first + 1).find(',');

        std::string after1_comma = line.substr(p_first + 1);
        /* verificam daca am gasit a doua virgula */
        if (p_second == std::string::npos) {
          std::cout << "Pe linia " << line_number
                    << " se afla o tranzitie invalida\n";
        } else { /* daca am gasit a doua virgula */
          /* daca virgula 2 nu se afla pe pozitia 1 in substr, atunci stim ca
           * litera din tranzitie este defapt un cuvant, contradictie cu
           * cerintele impuse de noi */
          if (p_second != 1) {
            std::cout << "Pe linia" << line_number
                      << " se afla o tranzitie invalida\n";
          } else { /* daca tranzitia are litera corecta */
            /* obtinem informatiile despre tranzitia curenta */
            int si = std::stoi(line.substr(0, p_first));
            int sf = std::stoi(after1_comma.substr(p_second + 1));
            char litera = line[p_first + 1];
            /* salvam tranzitia */
            transitions.push_back(std::tuple<int, char, int>(si, litera, sf));
          }
        }
      }
    }

    ++line_number;
  }

  fin.close(); /* inchidem fisierul dupa ce am terminat de citit configul */

  if (config_incorect) { /* daca am gasit erori in config */
    return false;
  }

  for (int i = 0; i < 3; i++) { /* verificam sa fi citit cele 3 stari */
    if (delimitare[i] != 1) {
      switch (i) {
      case 0:
        std::cout << "Nu exista Sigma\n";
        break;
      case 1:
        std::cout << "Nu exista Stari\n";
        break;
      case 2:
        std::cout << "Nu exista Tranzitii\n";
        break;
      default:
        break;
      }
      return false;
    }
  }

  if (fr_si != 1) { /* verificam sa fi gasit starea initiala */
    std::cout << "Nu exista stare initiala\n";
    return false;
  }

  /* daca am citit tot configul si nu am gasit erori sintactice returnam true */
  return true;
}

bool NFA::acceptaCuvant(std::string cuvant, int cs) {
  int current_state;

  if (cs == -1) { /* daca este primul call al apelului recursiv starea */
    current_state = this->initial_state; /* curenta va fi cea initiala */
  } else {
    current_state = cs;
  }

  bool ok = true; /* daca am gasit in arborele cu radacina in starea curenta un
                   * path care accetpa cuvantul */

  if (!cuvant.empty()) { /* daca mai avem litere in cuvant */
    /* verificam daca plecand cu orice tranzitie gasim o cale care sa ne
     * accepte cuvantul */
    bool gasit = false;

    for (const auto &transition : transitions) {
      /* daca putem avansa din starea curenta cu aceasta tranzitie */
      if (current_state == std::get<0>(transition) &&
          cuvant[0] == std::get<1>(transition)) {
        if (gasit == false) { /* daca inca nu am gasit un path bun */
          gasit = acceptaCuvant(cuvant.substr(1), std::get<2>(transition));
        }
      }
    }

    if (!gasit) { /* daca nu am gasit o tranzitie cu care sa plecam din */
      ok = false; /* starea curenta, atunci cuvantul nu este acceptat */
    }
  }

  if (cuvant.empty()) { /* daca am terminat de procesat cuvantul */
    /* daca starea curenta se afla in multimea starilor finale si am procesat
     * tot cuvantul */
    if (std::find(final_states.begin(), final_states.end(), current_state) !=
            final_states.end() &&
        ok) {
      return true;
    } else {
      return false;
    }
  } else {     /* returnam in apelul recursiv daca pathul pe care suntem */
    return ok; /* accepta cuvantul */
  }
}

DFA &NFA::transformToDFA() {
  /* informatiile despre dfa ul echivalent */
  std::vector<char> dfa_sigma;
  std::vector<int> dfa_states;
  int dfa_is;
  std::vector<int> dfa_final_states;
  std::vector<std::tuple<int, char, int>> dfa_transitions;

  /* alfabetul este acelasi */
  for (const auto &c : this->sigma) {
    dfa_sigma.push_back(c);
  }

  /* starea initiala va fi aceeasi, doar o vom nota diferit */
  dfa_is = 0;
  dfa_states.push_back(dfa_is);

  /* cautam ce stari vom mai avea in setul de stari si tranzitiile */
  bool ok{true};
  int new_state{1}; /* counter pentru noile stari */
  std::map<int, std::set<int>> convert_states;
  convert_states.insert(
      std::pair<int, std::set<int>>(dfa_is, {this->initial_state}));
  int cs{-1}; /* retinem la fiecare iteratie starea curenta pe care o
                     procesam */

  /* la fiecare iteratie ar trebui sa fie adaugate stari, daca nu se adauga
   * stari atunci am gasit toate starile dfa ului echivalent */
  do {
    cs++; /* procesam urmatoarea stare */
    std::set<int> equiv_states = convert_states[cs];

    /* pentru fiecare litera cu care putem pleca din multimea de stari din NFA
     * corespunzatoare starii curente in DFA */
    for (const auto &c : sigma) {
      std::set<int> destination_states;
      /* verificam daca vom adauga o tranzitie cs,c,d, unde d poate fi o stare
       * existenta sau una noua in DFA */
      bool exista_tranzitie{false};
      /* pentru fiecare stare din multimea de stari din NFA corespunzatoare
       * starii din DFA cautam tranzitiile */
      for (const auto &s : equiv_states) {
        /* plecam cu toate tranzitiile posibile, si salvam pentru fiecare litera
         * o lista cu starile in care ajungem */
        for (const auto &t : this->transitions) {
          /* daca am gasit o tranzitie cu care putem pleca din starea curenta */
          if (s == std::get<0>(t) && c == std::get<1>(t)) {
            destination_states.insert(std::get<2>(t));
            exista_tranzitie = true;
          }
        }
      }

      if (!exista_tranzitie) /* daca nu ajungem nicaieri plecand din */
        continue;            /* tranzitia curenta */

      /* daca ajungem in aceeasi stare din care am plecat in DFA, atunci nu mai
       * trebuie sa o adaugam inca o data */
      if (equiv_states == destination_states) {
        dfa_transitions.push_back(std::tuple<int, char, int>(cs, c, cs));
        continue;
      }

      /* am gasit o stare noua, deci o adaugam in setul de stari din AFN
       * corespunzatoare starii din AFD */
      convert_states.insert(
          std::pair<int, std::set<int>>(new_state, destination_states));
      /* adaugam si starea noua in setul de stari ale DFAului */
      dfa_states.push_back(new_state);

      /* verificam daca este stare finala
       * pentru a fi stare finala in DFA trebuie sa contina in
       * destination_states o stare finala din NFA */
      bool is_final{false};
      for (const auto &ds : destination_states) {
        for (const auto &fs : this->final_states) {
          if (ds == fs) {
            is_final = true;
            break; /* e suficient sa gasim doar o stare */
          }
        }
        if (is_final) /* daca starea ds este finala */
          break;
      }
      if (is_final) { /* daca este stare finala o adaugam in lista starilor
                         finale */
        dfa_final_states.push_back(new_state);
      }

      /* adaugam tranzitia in DFA */
      dfa_transitions.push_back(std::tuple<int, char, int>(cs, c, new_state));
      new_state++;
    }

    /* } while (new_state != convert_states.size()); */
  } while (new_state != cs);

  /* std::cout << "\nAFISARE DFA\n"; */
  /* for (const auto &s : dfa_sigma) */
  /*   std::cout << s << " "; */
  /* std::cout << '\n'; */
  /* for (const auto &s : dfa_states) */
  /*   std::cout << s << " "; */
  /* std::cout << '\n'; */
  /* for (const auto &s : dfa_final_states) */
  /*   std::cout << s << " "; */
  /* std::cout << '\n'; */
  /* for (const auto &t : dfa_transitions) */
  /*   std::cout << std::get<0>(t) << ',' << std::get<1>(t) << ',' */
  /*             << std::get<2>(t) << '\n'; */
  /* std::cout << '\n'; */

  DFA *equiv_dfa =
      new DFA(dfa_sigma, dfa_states, dfa_is, dfa_final_states, dfa_transitions);
  return *equiv_dfa;
}
