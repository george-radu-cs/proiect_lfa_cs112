/* test for cfg reduction */
#include "CFG.h"

int main(int argc, char **argv) {
  CFG *cfg = new CFG(argv[1]);

  if (cfg->isCreated()) {
    std::cout << "The initial CFG:\n" << *cfg << '\n';
    cfg->simplifyCFG();
    std::ofstream fout(argv[2]);
    fout << *cfg;
    std::cout << "The simplified CFG:\n" << *cfg << '\n';
  }

  delete cfg;

  return 0;
}
