#include <vector>
#include<string>

std::vector<int> grid_from_string(const std::string& input) {
  std::vector<int> grid;
  grid.reserve(input.length());

  for (char c : input) {
    if (c == '.' || c == '0') {
      grid.push_back(0);
    } else if (c >= '1' && c <= '9') {
      grid.push_back(c - '0');
    } else {
      std::cerr << "Error: Invalid character '" << c << "' in input.\n";
      return {};
    }
  }
  return grid;
}