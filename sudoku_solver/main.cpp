#include <algorithm>  // use -std=c++20 for std::ranges
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <unordered_set>
#include <vector>

// constants
/* const std::vector<std::vector<char>> INPUT = { */
/*     {'5','3','.','.','7','.','.','.','.'}, */
/*     {'6','.','.','1','9','5','.','.','.'}, */
/*     {'.','9','8','.','.','.','.','6','.'}, */
/*     {'8','.','.','.','6','.','.','.','3'}, */
/*     {'4','.','.','8','.','3','.','.','1'}, */
/*     {'7','.','.','.','2','.','.','.','6'}, */
/*     {'.','6','.','.','.','.','2','8','.'}, */
/*     {'.','.','.','4','1','9','.','.','5'}, */
/*     {'.','.','.','.','8','.','.','7','9'} */
/* }; */
const std::vector<std::vector<char>> INPUT = {
    {'.', '.', '9', '7', '4', '8', '.', '.', '.'},
    {'7', '.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '2', '.', '1', '.', '9', '.', '.', '.'},
    {'.', '.', '7', '.', '.', '.', '2', '4', '.'},
    {'.', '6', '4', '.', '1', '.', '5', '9', '.'},
    {'.', '9', '8', '.', '.', '.', '3', '.', '.'},
    {'.', '.', '.', '8', '.', '3', '.', '2', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.', '6'},
    {'.', '.', '.', '2', '7', '5', '9', '.', '.'}};

// Data definitions
struct Cell {
  Cell();
  explicit Cell(char c);
  explicit Cell(int i);
  int operator<=>(Cell const &other) const = default;

  std::unordered_set<int> vals = {1, 2, 3, 4, 5, 6, 7, 8, 9};
};
using Board = std::array<Cell, 81>;
namespace ranges = std::ranges;

// function declarations
Board makeBoard(std::vector<std::vector<char>> const &input);
Board eliminateDupes(Board const &board);
Board completeColumns(Board const &board);
bool isSolved(Board const &board);
Board solveSimulatedAnnealing(Board const &board);
int makeIndex(int row, int col);
std::ostream &operator<<(std::ostream &os, Cell const &cell);
std::ostream &operator<<(std::ostream &os, Board const &board);

int main() {
  Board board = makeBoard(INPUT);

  std::cout << board << '\n';

  // This will reduce the number of guesses we have to make
  Board prev = board;
  while (not isSolved(board)) {
    board = eliminateDupes(board);
    board = completeColumns(board);

    if (board == prev) {
      break;
    } else {
      prev = board;
    }
  }

  board = solveSimulatedAnnealing(board);

  std::cout << board << '\n';
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//                          implementation below
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Board solveSimulatedAnnealing(Board const &board) { return board; }

bool isSolved(Board const &board) {
  return not ranges::any_of(
      board, [](Cell const &cell) { return cell.vals.size() > 1; });
}

int makeIndex(int row, int col) { return row * 9 + col; }

Board makeBoard(std::vector<std::vector<char>> const &input) {
  Board board;

  for (int row = 0; row < 9; row++) {
    for (int col = 0; col < 9; col++) {
      int index = makeIndex(row, col);
      board[index] = Cell(input[row][col]);
    }
  }

  return board;
}

// This will normalize a row or column number to the "upper-left" quadrant of
// the sudoku square
int upperLeft(int val) {
  if (val < 3) {
    return 0;
  } else if (val < 6) {
    return 1;
  } else {
    return 2;
  }
}

Board eliminateDupes(Board const &data) {
  Board board(data);

  for (int row = 0; row < 9; row++) {
    for (int col = 0; col < 9; col++) {
      int index = makeIndex(row, col);
      Cell &cell = board[index];
      if (cell.vals.size() > 1) {
        // update based on row
        for (int _col = 0; _col < 9; _col++) {
          Cell &_cell = board[makeIndex(row, _col)];

          if (col != _col && _cell.vals.size() == 1) {
            cell.vals.erase(*_cell.vals.begin());
          }
        }

        // update based on col
        for (int _row = 0; _row < 9; _row++) {
          Cell &_cell = board[makeIndex(_row, col)];

          if (row != _row && _cell.vals.size() == 1) {
            cell.vals.erase(*_cell.vals.begin());
          }
        }

        // update based on square
        int first = upperLeft(row) * 27 + upperLeft(col) * 3;

        for (int _inc = 0; _inc < 3; _inc++) {
          int _index = first + _inc * 9;
          for (int _add = 0; _add < 3; _add++) {
            int _finalIndex = _index + _add;
            Cell &_cell = board[_finalIndex];
            if (index != _finalIndex && _cell.vals.size() == 1) {
              cell.vals.erase(*_cell.vals.begin());
            }
          }
        }
      }
    }
  }

  return board;
}

Board completeColumns(Board const &data) {
  Board board(data);

  for (int col = 0; col < 9; col++) {
    std::unordered_set<int> still_need = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    // clean up what we already have
    for (int row = 0; row < 9; row++) {
      Cell const &cell = board[makeIndex(row, col)];

      if (cell.vals.size() == 1) {
        still_need.erase(*cell.vals.begin());
      }
    }

    // if only one cell has it, then we can solve it
    for (int const check : still_need) {
      std::vector<int> matches;
      /* std::cout << "still need " << check << '\n'; */
      for (int row = 0; row < 9; row++) {
        Cell const &cell = board[makeIndex(row, col)];

        if (cell.vals.contains(check)) {
          /* std::cout << "found match in row " << row << '\n'; */
          matches.push_back(row);
        }
      }

      if (matches.size() == 1) {
        board[makeIndex(matches[0], col)].vals = {check};
      }
    }
  }

  return board;
}

Cell::Cell() {}

Cell::Cell(char c) {
  if (c != '.') {
    vals = {(int)c - 48};
  }
};
Cell::Cell(int i) : vals({i}){};

std::ostream &operator<<(std::ostream &os, Cell const &cell) {
  std::ostringstream out;

  if (cell.vals.size() > 1) {
    out << '(';
    int i = 0;
    for (auto const &val : cell.vals) {
      out << val;

      if (i < cell.vals.size() - 1) {
        out << ",";
      } else {
        out << ')';
      }

      i++;
    }
  } else {
    out << (char)(*cell.vals.begin() + 48);
  }

  os << std::setw(12) << out.str();
  return os;
}

std::ostream &operator<<(std::ostream &os, Board const &board) {
  std::string rule(125, '-');
  os << rule << '\n';
  for (int i = 0; i < board.size(); i++) {
    Cell const &cell = board.at(i);
    if (i % 9 == 0) {
      os << '|';
    }
    os << cell;
    if (i % 3 == 2) {
      os << "|";
    } else {
      os << ", ";
    }
    if (i % 9 == 8) {
      os << "\n";
    }
    if (i % 27 == 26) {
      os << rule << '\n';
    }
  }
  return os;
}
