#pragma once
#include "exact_cover_solver.h"

inline ExactCoverMatrix build_exact_cover_problem(
    const std::vector<int>& grid) {
  const int n = 9, k = 3;

  /// up to 729 options
  /// 81*9 (eg put a "1" in cell 0,0)
  ///  some options are excluded by the grid

  ///
  /// 324 constraints
  /// 81 * there must be exactly one number in the cell
  /// 81 * there must be exactly one "1" (or v) in the column
  /// 81 * there must be exactly one "1" (or v) in the row
  /// 81 * there must be exactly one "1" (or v) in the block
  /// =81*4 = 324
  ///
  /// up to 2916 constraint contributions
  /// each option makes a contribution to 4 constaints
  /// 729*4 = 2916

  ExactCoverMatrix result(324, 2916);

  auto add_row = [&](int r, int c, int v) {
    int option_number = r * 81 + c * 9 + v;

    int cell_constraint = r * 9 + c;
    int row_constraint = r * 9 + v;
    int col_constraint = c * 9 + v;
    int block = (r / 3) * 3 + (c / 3);
    int block_constraint = block * 9 + v;
    vector<int> constraints = {cell_constraint, 81 + row_constraint ,
                               162 + col_constraint, 243 + block_constraint};

    result.add_row(option_number, constraints);
  };

  /// add each option as a row to the matrix
  for (int row = 0; row < 9; row++) {
    for (int col = 0; col < 9; col++) {
      int index = 9 * row + col;
      if (grid[index] != 0) {
        // if the value is specified in the grid, it is the only option
        int v = grid[index] - 1;
        add_row(row, col, v);

      } else {
        // otherwise there are 9 options
        for (int v = 0; v < 9; v++) {
          add_row(row, col, v);
        }
      }
    }
  }
  return result;
}

inline std::vector<int> interpret_exact_cover_solution(
    const std::vector<int>& solution) {
  std::vector<int> result(81);
  for (int i : solution) {
    int v = i % 9;
    int c = (i / 9) % 9;
    int r = i / 81;
    result[r * 9 + c] = v + 1;
  }
  return result;
}