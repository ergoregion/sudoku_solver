#include "exact_cover_solver.h"
#include <climits>

ExactCoverMatrix::ExactCoverMatrix(int columns, int max_tile_count)
    : _column_sizes(columns, 0), _column_heads(columns) {
  // set the null root node
  _root = {&_root, &_root, &_root, &_root, nullptr, -1, -1};

  // build header row
  ExactCoverTile* prev = &_root;
  for (int i = 0; i < columns; ++i) {
    ExactCoverTile& col = _column_heads[i];
    col = {prev, &_root, &col, &col, &col, -1, i};
    col.left->right = &col;
    col.right->left = &col;
    prev = &col;
  }

  // reserve enough tiles to avoid reallocating the storage vector
  _tiles_storage.reserve(max_tile_count);
}

void ExactCoverMatrix::add_row(int r, const vector<int>& cols) {
  // add one tile per col

  ExactCoverTile* row_head = nullptr;
  ExactCoverTile* prev = nullptr;
  for (int c : cols) {
    ExactCoverTile* column_head = &_column_heads[c];

    // create a new tile at the end of the column
    _tiles_storage.emplace_back(nullptr, nullptr, column_head->up, column_head,
                                column_head, r, c);
    _column_sizes[c]++;

    ExactCoverTile* new_tile = &_tiles_storage.back();

    // flag if this is the head of the row
    if (row_head == nullptr) {
      row_head = new_tile;
      prev = new_tile;
    }

    // fix the horizontal linking
    new_tile->left = prev;
    new_tile->right = row_head;

    // fix the double linking
    new_tile->left->right = new_tile;
    new_tile->right->left = new_tile;
    new_tile->up->down = new_tile;
    new_tile->down->up = new_tile;

    prev = new_tile;
  }
}
vector<int> ExactCoverMatrix::solve() {
  vector<int> result;
  _solve_remaining(result);
  return result;
}
void ExactCoverMatrix::_cover(ExactCoverTile* column_head) {
  // make the other comumn headers forget this column
  column_head->right->left = column_head->left;
  column_head->left->right = column_head->right;
  // note that we havn't forgotten them, we can re-insert outselves

  // loop over the colum, covering each applicable row
  for (auto tile_of_col = column_head->down; tile_of_col != column_head;
       tile_of_col = tile_of_col->down) {
    _cover_a_row(tile_of_col);
  }
}

void ExactCoverMatrix::_uncover(ExactCoverTile* column_head) {
  // loop over the colum, uncovering each applicable row
  for (auto tile_of_col = column_head->up; tile_of_col != column_head;
       tile_of_col = tile_of_col->up) {
    _uncover_a_row(tile_of_col);
  }

  // reinsert outself to the linked list
  column_head->left->right = column_head;
  column_head->right->left = column_head;
}

void ExactCoverMatrix::_cover_a_row(ExactCoverTile* row_tile) {
  for (auto tile_of_row = row_tile->right; tile_of_row != row_tile;
       tile_of_row = tile_of_row->right) {
    // make the rest of the colum forget this tile
    tile_of_row->down->up = tile_of_row->up;
    tile_of_row->up->down = tile_of_row->down;
    // note that we havn't forgotten them, we can re-insert outselves
    _column_sizes[tile_of_row->col_index]--;
  }
}

void ExactCoverMatrix::_uncover_a_row(ExactCoverTile* row_tile) {
for (auto tile_of_row = row_tile->left; tile_of_row != row_tile;
       tile_of_row = tile_of_row->left) {
    _column_sizes[tile_of_row->col_index]++;
    //reinsert this tile to the linked list
    tile_of_row->up->down = tile_of_row;
    tile_of_row->down->up = tile_of_row;
  }
}

bool ExactCoverMatrix::_solve_remaining(vector<int>& solution) {
  // check if all colums covered
  if (_root.right == &_root) return true;
  auto sc = _shortest_column();
  // check a shortest column exists
  if (sc == nullptr) return false;
  // if the size is 0, there is now an uncoverable colum
  if (_column_sizes[sc->col_index] == 0) return false;

  _cover(sc);

  // try each of the rows of this columm,
  for (auto tile_of_col = sc->down; tile_of_col != sc;
       tile_of_col = tile_of_col->down) {
    solution.push_back(tile_of_col->row_index);

    for (auto j = tile_of_col->right; j != tile_of_col; j = j->right)
      _cover(j->column_head);

    if (_solve_remaining(solution)) return true;

    for (auto j = tile_of_col->left; j != tile_of_col; j = j->left)
      _uncover(j->column_head);

    solution.pop_back();
  }

  _uncover(sc);
  return false;
}

ExactCoverTile* ExactCoverMatrix::_shortest_column() const {
  ExactCoverTile* col = nullptr;
  int minSize = INT_MAX;
  for (ExactCoverTile* c = _root.right; c != &_root; c = c->right) {
    int sz = _column_sizes[c->col_index];
    if (sz < minSize) {
      minSize = sz;
      col = c;
      if (minSize == 0) break;
    }
  }
  return col;
}
