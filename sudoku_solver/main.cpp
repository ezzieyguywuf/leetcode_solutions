#include <algorithm> // use -std=c++20 for std::ranges
#include <iostream>
#include <ostream>
#include <unordered_set>
#include <vector>

const std::vector<std::vector<char>> INPUT = {
    {'5','3','.','.','7','.','.','.','.'},
    {'6','.','.','1','9','5','.','.','.'},
    {'.','9','8','.','.','.','.','6','.'},
    {'8','.','.','.','6','.','.','.','3'},
    {'4','.','.','8','.','3','.','.','1'},
    {'7','.','.','.','2','.','.','.','6'},
    {'.','6','.','.','.','.','2','8','.'},
    {'.','.','.','4','1','9','.','.','5'},
    {'.','.','.','.','8','.','.','7','9'}
};

// Data definitions
struct Cell{
    Cell();
    explicit Cell(char c);
    explicit Cell(int i);
    char toChar() const;

    std::unordered_set<int> vals = {1, 2, 3, 4, 5, 6, 7, 8, 9};
};
using Board = std::array<Cell, 81>;

// function declarations
Board makeBoard(std::vector<std::vector<char>> const& input);
std::ostream& operator<<(std::ostream& os, Cell const& cell);
std::ostream& operator<<(std::ostream& os, Board const& board);

int main() {
    Board board = makeBoard(INPUT);

    std::cout << board << '\n';
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//                          implementation below
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

/** A single cell on the game board.
 *
 *  A cell is "solved" when the set of potential values contains only a single
 *  number
 */
int makeIndex(int row, int col)
{
    return row * 9 + col;
}

Board makeBoard(std::vector<std::vector<char>> const& input)
{
    Board board;

    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            int index = makeIndex(row, col);
            board[index] = Cell(input[row][col]);
        }
    }

    return board;
}

Cell::Cell(){}

Cell::Cell(char c) {
    if (c != '.') {
        vals = {(int) c - 48};
    }
};
Cell::Cell(int i) : vals({i}){};

char Cell::toChar() const {
    return vals.size() > 1
            ? '.'
            : (char) (*vals.begin() + 48);
};

std::ostream& operator<<(std::ostream& os, Cell const& cell)
{
    os << cell.toChar();
    return os;
}

std::ostream& operator<<(std::ostream& os, Board const& board)
{
    for (int i = 0; i < board.size(); i++) {
        Cell const& cell = board.at(i);
        if (i % 9 == 0) {
            os << '[';
        }
        os << cell;
        if (i % 9 == 8) {
            os << "]\n";
        }
        else {
            os << ", ";
        }
    }
    return os;
}
