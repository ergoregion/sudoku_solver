CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra

SRC = sudoku_solver.cpp exact_cover_solver.cpp
TARGET = sudoku_solver

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)