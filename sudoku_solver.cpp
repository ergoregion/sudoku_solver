#include <vector>
#include <iostream>
#include <cstdlib>
#include "line_parser.h"
#include "build_exact_cover_problem.h"

using namespace std;

void solve_once(const std::string& input) {
  int n = 9;
  std::vector<int> grid = grid_from_string(input);
  auto b = build_exact_cover_problem(grid);
  auto solved = b.solve();

  if (solved.size() == 81) {
    for (int v : interpret_exact_cover_solution(solved)) {
      cout << v;
    }
    cout << endl;
  } else {
    std::cout << "solve failed" << endl;
  }
}

int main(int argc, char* argv[]) {
  
  std::string input;
  if (argc == 2) {
    input = argv[1];
    if (input.length() != 81) {
      std::cerr << "Error: Input must be exactly 81 characters.\n";
      return 1;
    }
    solve_once(input);
  } else {
    while (std::getline(std::cin, input)) {
      if (input.empty()) continue;
      solve_once(input);
    }
    return 0;
  }
}
