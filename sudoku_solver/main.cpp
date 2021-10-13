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
using IndexRow = std::array<std::size_t, 9>;
using Indices = std::array<IndexRow, 9>;

// function declarations
Board makeBoard(std::vector<std::vector<char>> const& input);
constexpr Indices makeIndices(bool makeRows = true);
constexpr Indices makeSquareIndices();
std::ostream& operator<<(std::ostream& os, Cell const& cell);
std::ostream& operator<<(std::ostream& os, Board const& board);


int main() {
    Board board = makeBoard(INPUT);
    /* Indices rows = makeIndices(); */
    /* Indices cols = makeIndices(false); */
    /* Indices sqrs = makeSquareIndices(); */

    std::cout << board << '\n';
    /* printTable(rows); */
    /* printTable(cols); */
    /* printTable(sqrs); */
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

constexpr Indices makeIndices(bool makeRows) {
    Indices out;

    for(int i = 0; i < 9; i++) {
        IndexRow row;
        for (int j = 0; j < 9; j++) {
            if (makeRows) {
                row[j] = i * 9 + j;
            }
            else {
                row[j] = i + j * 9;
            }
        }

        out[i] = row;
    }

    return out;
}

constexpr Indices makeSquareIndices()
{
    Indices out;

    for(int i = 0; i < 9; i++) {
        IndexRow sqr;
        for (int j = 0; j < 9; j++) {
            int index = 3 * (i % 3) + 9 * (j % 3);
            if (j >= 3 && j < 6) {
                index += 1;
            }
            else if (j >=6) {
                index += 2;
            }
            if (i >= 3 && i < 6) {
                index += 27;
            }
            else if (i >= 6) {
                index += 54;
            }
            sqr[j] = index;
        }

        out[i] = sqr;
    }

    return out;
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
