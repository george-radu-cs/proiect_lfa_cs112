#include "DFA.h"

DFA::DFA(std::string file_name) {
  if (citireValidare(file_name)) {
    if (validareDate()) {
      std::cout << "Datele sunt valide, a fost creat DFA-ul\n";
      creat_corect = true;
    } else {
      std::cout << "Datele nu sunt valide, nu a fost creat DFA-ul\n";
      creat_corect = false;
    }
  } else {
    std::cout << "Nu a fost creat DFA-ul. Obiectul creat poate contine erori "
                 "in informatie\n";
    creat_corect = false;
  }
}

DFA::DFA(const std::vector<char> &sigma, const std::vector<int> &states,
         const int is, const std::vector<int> &final_states,
         const std::vector<std::tuple<int, char, int>> &transitions) {
  size_t lungime_vector;
  lungime_vector = sigma.size();
  for (int i = 0; i < lungime_vector; i++) {
    this->sigma.push_back(sigma[i]);
  }
  lungime_vector = states.size();
  for (int i = 0; i < lungime_vector; i++) {
    this->states.push_back(states[i]);
  }
  this->initial_state = is;
  lungime_vector = final_states.size();
  for (int i = 0; i < lungime_vector; i++) {
    this->final_states.push_back(final_states[i]);
  }
  lungime_vector = transitions.size();
  int s1, s2;
  char l;
  for (int i = 0; i < lungime_vector; i++) {
    s1 = std::get<0>(transitions[i]);
    s2 = std::get<2>(transitions[i]);
    l = std::get<1>(transitions[i]);
    this->transitions.push_back(std::tuple<int, char, int>(s1, l, s2));
  }
  if (this->validareDate()) {
    this->creat_corect = true;
    std::cout << "Datele sunt valide, a fost creat DFA-ul\n";
  }
}

bool DFA::validareDate() {
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

  /* verificam ca nu plecam din aceeasi stare cu o litera de mai multe ori */
  for (const auto &t1 : transitions) {
    for (const auto &t2 : transitions) {
      if (std::get<0>(t1) == std::get<0>(t2) &&
          std::get<1>(t1) == std::get<1>(t2) &&
          std::get<2>(t1) != std::get<2>(t2)) {
        std::cout << "Configul este un NFA.\n";
        return false;
      }
    }
  }

  return true;
}

bool DFA::citireValidare(std::string file_name) {
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

bool DFA::acceptaCuvant(std::string cuvant) {
  int current_state = this->initial_state;
  bool ok = true;

  int index = 0;
  int lungime_cuvat = cuvant.length();
  while (index < lungime_cuvat) {
    bool gasit = false;
    for (const auto &transtion : transitions) {
      if (current_state == std::get<0>(transtion) &&
          cuvant[index] == std::get<1>(transtion)) {
        current_state = std::get<2>(transtion);
        gasit = true;
        break;
      }
    }

    /* daca nu am gasit o tranzitie cu care sa plecam din starea curenta,
     * atunci cuvantul nu este acceptat */
    if (!gasit) {
      ok = false;
      break;
    }

    ++index;
  }

  /* daca starea curenta se afla in multimea starilor finale si am procesat
   * tot cuvantul */
  //  std::cout << current_state;
  if (std::find(final_states.begin(), final_states.end(), current_state) !=
          final_states.end() &&
      ok) {
    std::cout << "Cuvantul a fost acceptat.\n";
    return true;
  } else {
    std::cout << "Cuvantul nu a fost acceptat.\n";
    return false;
  }
}

void DFA::scrieConfig(std::string file_name) {
  std::ofstream fout(file_name);

  /* scriem alfabetul */
  fout << "Sigma:\n";
  for (const auto &c : sigma) {
    fout << c << '\n';
  }
  fout << "End\n";

  /* scriem starile */
  fout << "States:\n";
  for (const auto &s : states) {
    fout << s;
    bool need_comma{true};
    if (s == initial_state) {
      fout << ",S";
      need_comma = false;
    }
    for (const auto &fs : final_states) {
      if (fs == s) {
        if (need_comma) {
          fout << ",F";
        } else {
          fout << "F";
        }
      }
    }
    fout << '\n';
  }
  fout << "End\n";

  /* scriem tranzitiile */
  fout << "Transitions:\n";
  for (const auto &t : transitions) {
    fout << std::get<0>(t) << ',' << std::get<1>(t) << ',' << std::get<2>(t)
         << '\n';
  }
  fout << "End\n";

  fout.close();
}

DFA &DFA::minimizareDFA() {
  /* informatiile despre dfa-ul simplificat */
  std::vector<int> dfa_states;
  int dfa_is;
  std::vector<int> dfa_final_states;
  std::vector<std::tuple<int, char, int>> dfa_transitions;

  /* construim matricea triunghiulara de stari */
  int nr_stari = this->states.size();
  bool **M = new bool *[nr_stari];
  for (int i = 0; i < nr_stari; i++)
    M[i] = new bool[i];
  /* o initializam pe toate cu false */
  for (int i = 1; i < nr_stari; i++) {
    for (int j = 0; j <= i; j++)
      M[i][j] = false;
  }

  /* marcam starile separabile prin cuvantul vid lambda,
   * 2 stari sunt separabile daca una daca exista un cuvant pentru care plecan
   * din cele doua stari ajungem intr-o stare finala si intr-una nefinala*/
  for (const auto &fs : this->final_states) { /* vrem o stare finala */
    for (const auto &s : this->states) {      /* cautam in restul de stari */
      bool is_final{false};                   /* o stare nefinala */
      for (const auto &f : this->final_states) {
        if (s == f) {
          is_final = true;
          break;
        }
      }

      if (is_final) { /* daca ambele stari sunt finale => sunt echivalente */
        continue;     /* mergem mai departe, nu am gasit ce cautam */
      } else { /* am gasit o pereche de stari separabile, o salvam in M */
        if (fs < s) {
          M[s][fs] = true;
        } else {
          M[fs][s] = true;
        }
      }
    }
  }

  bool marked_sep_pair{true}; /* retinem daca am gasit 2 stari separabile */
  /* mergi la pasul k daca la pasul k-1 s-a marcat in tabel cel putin o
   * pereche de stari separabile */
  while (marked_sep_pair) {
    marked_sep_pair = false;
    for (int i = 1; i < nr_stari; i++) { /* pentru fiecare pereche din */
      for (int j = 0; j < i; j++) {      /* matricea triunghiulara de stari */
        if (!M[i][j]) { /* daca starile i,j nu sunt separabile */
          /* pentru fiecare simbol c din alfabetul sigma */
          for (const auto &c : this->sigma) {
            int ds1, ds2; /* retinem in ce stari vom ajunge cu orice litera */
            for (const auto &t : this->transitions) {
              if (states[i] == std::get<0>(t) && c == std::get<1>(t)) {
                ds1 = std::get<2>(t);
              }
              if (states[j] == std::get<0>(t) && c == std::get<1>(t)) {
                ds2 = std::get<2>(t);
              }
            }

            if (ds1 < ds2) { /* ds1 trebuie sa fie mereu mai mare decat ds2 */
              int temp = ds1;
              ds1 = ds2;
              ds2 = temp;
            }

            /* daca am ajuns intr-o pereche de stari separabile */
            if (M[ds1][ds2]) {
              M[i][j] = true; /* marcam starile i,j ca fiind separabile */
              marked_sep_pair = true; /* retinem ca am gasit 2 stari sep */
              break; /* nu mai trebuie sa cautam cu alt simbol din alfabet */
            }
          }
        }
      }
    }
  }

  /* vom corela fiecare stare din DFA ul minimizat cu un set de stari din DFA ul
   * vechi */
  std::map<int, std::set<int>> state_group_states;
  int new_state{0};
  /* pentru fiecare stare incercam sa vedem daca face parte in DFA ul
   * minimizat dintr-un grup de stari sau este individuala */
  while (new_state < nr_stari) {
    std::set<int> group_states; /* retinem grupul de stari asociat noii stari */
    bool is_grouped{false};     /* retinem daca noua stare este formata dintr-un
                                   grup de stari vechi */
    /* cautam pe linia starii new_state */
    for (int i = 0; i < new_state; i++) {
      if (!M[new_state][i]) { /* daca am gasit 2 stari echivalente */
        group_states.insert(states[i]);
        is_grouped = true;
      }
    }

    /* cautam pe coloana starii new_state */
    for (int i = new_state + 1; i < nr_stari; i++) {
      if (!M[i][new_state]) { /* daca am gasit 2 stari echivalente */
        group_states.insert(states[i]);
        is_grouped = true;
      }
    }

    group_states.insert(states[new_state]); /* adaugam starea noua in grup */

    bool exista_deja{false};
    for (const auto &p : state_group_states) {
      if (group_states == std::get<1>(p)) {
        exista_deja = true;
        break;
      }
    }

    if (!exista_deja) { /* daca nu avem deja starea */
      /* adaugam legatura dintre noua stare si grupul echivalent de stari */
      state_group_states.insert(
          std::pair<int, std::set<int>>(new_state, group_states));
      dfa_states.push_back(new_state); /* adaugam starea noua in vectorul
                                          starilor DFA-ului minimizat */
    }

    new_state++; /* incercam sa construim o noua stare */
  }

  /* cautam starea initiala si starile finale */
  for (const auto &p : state_group_states) {
    for (const auto &s : std::get<1>(p)) { /* pentru fiecare stare din grup */
      if (s == this->initial_state) {      /* verificam daca grupul de stari *
                                            * contine starea initiala */
        dfa_is = std::get<0>(p);           /* si o adaugam */
      }

      /* verificam daca grupul de stari contine cel putin o stare finala,
       * caz in care starea curenta din DFA-ul minimizat este si ea finala */
      for (const auto &fp : final_states) {
        if (s == fp) {
          dfa_final_states.push_back(s); /* adaugam in vectorul de stari finale
                                            ale DFA-ului minimizat */
          break;
        }
      }
    }
  }

  /* stabilim tranzitiile in DFA-ul minimizat */
  for (const auto &p : state_group_states) { /* pentru fiecare pereche */
    std::set<int> group_states = std::get<1>(p);
    int s = *group_states.begin(); /* avem nevoie doar de o stare din grup */

    /* verificam tranzitiile lui s */
    for (const auto &t : transitions) {
      if (std::get<0>(t) == s) {
        bool loop_transition{false}; /* retinem daca avem o tranzitie loop */
        /* daca ajungem din starea s cu tranzitia t intr-o stare din group
         * group_states atunci o sa avem in DFA-ul minimizat tranzitia
         * echivalenta s,t(1),s */
        for (const auto &stare : group_states) {
          if (std::get<2>(t) == stare) {
            dfa_transitions.push_back(std::tuple<int, char, int>(
                std::get<0>(p), std::get<1>(t), std::get<0>(p)));
            loop_transition = true;
            break;
          }
        }

        /* daca starea in care ajungem nu face parte din acelasi grup */
        if (!loop_transition) {
          for (const auto &pr : state_group_states) {
            bool found{false};
            for (const auto &stare : std::get<1>(pr)) {
              if (stare == std::get<2>(t)) {
                dfa_transitions.push_back(std::tuple<int, char, int>(
                    std::get<0>(p), std::get<1>(t), std::get<0>(pr)));
                found = true;
                break;
              }
            }
            if (found) /* trebuie sa gasim doar o stare din grup nu pe toate */
              break;
          }
        }
      }
    }
  }

  /* clean up la matricea triunghiulara */
  for (int i = 0; i < nr_stari; i++)
    delete[] M[i];
  delete[] M;

  /* construim dfa-ul simplificat cu datele calculate anterior */
  DFA *simplified_dfa = new DFA(this->sigma, dfa_states, dfa_is,
                                dfa_final_states, dfa_transitions);

  return *simplified_dfa;
}
