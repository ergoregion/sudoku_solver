# sudoku_solver
A simple sudoku solver created for Bear Challenge 2026


# Methodology

This treats the sudoku problem as an exact cover problem, where:

### Options

Up to 729 options.
+ Each digit (1->9) can be placed in each cell (81).
729 = 81*9 (eg put a "1" in cell 0,0).
Some options are excluded by the starting cells of the puzzle.

### Constraints

324 constraints
+ 81* there must be exactly one number in the cell.
+ 81* there must be exactly one "1" (or v) in the column.
+ 81* there must be exactly one "1" (or v) in the row.
+ 81* there must be exactly one "1" (or v) in the block.
=81*4 = 324

# Solving

We create a boolean matrix where the rows are the options, the columns are the constraints.

The values in the matrix are the boolean of whether the option would resolve the constraint.

Then as an exact cover problem we select a combination of rows such that each constraint is satisfied once and only once.

### Implementation

We implement the matrix as a sparse, 2d doubly linked list type grid. 
The matrix is stored as Quad-linked-list matrix element, referred to as tiles.
Tiles only exist where the value in the matrix is true.
Tiles have 4 links to other tiles. tiles which exist in all 4 cardinal directions, the first and last tiles circle like a toroid.


There is the option to cover rows and columns, by disconnecting their links to the rest of the gird. 
They can later be uncovered by reconnecting the links. This acts as a fast way to shrink the matrix.
See https://arxiv.org/abs/cs/0011047.

When a row is selected as true, that row satisfies some of the constraints. 
All other rows which would now satisfy the constraints are impossible, so they are covered.
At this point the constraint is met, and it's implication are represented by the covered rows, so the constraint column can also be covered.

All of these actions shrink the matrix, making subsequent steps faster, and motivating the next choice of row to select.

# Building 

A Makefile file is provided.
call with make, provided g++ is available.

# Usage

Arrange your sudoku problem(s) as a single line of 81 characters. Use `0` or `.` to represent the unknown items.
For example 
`18....64...5..48.3..67.............9.......3..47..5....51...4..3..9.7..58..4...9.`

### Method 1

Call the compiled executable with a single argument of a single sudoku. 
This one puzzle will be solved, the solution printed, then the program will end.

### Method 2

Call the compiled executable with no argument. Then pass problems indefinitely to the `stdin`. 
Solutions will be printed indefinitely to the `stdout`.