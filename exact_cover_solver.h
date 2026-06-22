#pragma once
#include <vector>
using namespace std;

/// <summary>
/// Single tile of an exact cover matrix
/// Quad-linked-list matrix element
/// The existence of this tile in the matrix indicates a true there
/// There are raw pointers to the tiles on all 4 sides
/// There is a short-cut pointer to the head of the col for speed
/// The row and col are stored so they don't need to be deduced
/// </summary>
struct ExactCoverTile
{
  ExactCoverTile *left, *right, *up, *down;
  ExactCoverTile* column_head;
  int row_index;
  int col_index;
};

/// <summary>
/// exact cover matrix
/// Quad-linked-list matrix
/// The existence of elements means that their value is 1
/// </summary>
class ExactCoverMatrix {
 public:
  ExactCoverMatrix(int columns, int max_tile_count);
  void add_row(int r, const vector<int>& cols);

  // solve the exact cover problem, returning the list of rows to use
  vector<int> solve();

 private:
  // covering an uncovering columns during a solve is a fast way to shrink and
  // regrow the matrix
  void _cover(ExactCoverTile* column_head);
  void _uncover(ExactCoverTile* column_head);

  // a part of covering colums is ccoveing the rows associated with them
  // (shrinking other colums)
  void _cover_a_row(ExactCoverTile* row_tile);
  void _uncover_a_row(ExactCoverTile* row_tile);

  // the recursive call to carry out the solve
  bool _solve_remaining(vector<int>& solution);

  ExactCoverTile* _shortest_column() const;

  // the root tile of the matrix
  ExactCoverTile _root;
  // The head tiles of each col
  vector<ExactCoverTile> _column_heads;
  // Cache of the current sizes of all the columns
  vector<int> _column_sizes;

  // Ownership of all the tiles so they don't go out of scope and lead to memory
  // leaks
  vector<ExactCoverTile> _tiles_storage;
};