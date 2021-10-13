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
struct Cell;
using Board = std::vector<std::vector<Cell>>;
using IndexRow = std::array<std::size_t, 9>;
using Indices = std::array<IndexRow, 9>;
namespace ranges = std::ranges;

// function declarations
Board makeBoard(std::vector<std::vector<char>> const& input);
template<typename Container> void printRow(Container const& row);
template<typename Container> void printTable (Container const& table);
constexpr Indices makeRowIndices();
std::ostream& operator<<(std::ostream& os, Cell const& cell);


int main() {
    Board board = makeBoard(INPUT);
    Indices rows = makeRowIndices();

    printTable(board);
    printTable(rows);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//                          implementation below
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

/** A single cell on the game board.
 *
 *  A cell is "solved" when the set of potential values contains only a single
 *  number
 */
struct Cell
{
    Cell(){}
    explicit Cell(int i) : vals({i}){}
    char toChar() const {
        return vals.size() > 1
                ? '.'
                : (char) (*vals.begin() + 48);
    }
    std::unordered_set<int> vals = {1, 2, 3, 4, 5, 6, 7, 8, 9};
};

Board makeBoard(std::vector<std::vector<char>> const& input)
{
    Board board;

    auto convertRow = [&board](std::vector<char> row) {
        std::vector<Cell> newRow;
        ranges::for_each(row, [&newRow](char const& c) {
            if (c == '.') {
                newRow.push_back(Cell());
            }
            else {
                // ascii for number '1' is 49
                newRow.push_back(Cell((int) c - 48));
            }
        });
        board.push_back(newRow);
    };
    ranges::for_each(input, convertRow);

    return board;
}

template<typename Container>
void printRow(Container const& row)
{
    for(auto it = row.begin(); it != row.end(); it++) {
        auto const& t = *it;
        std::cout << t << (it == row.end() - 1 ? "" : ", ");
    }
}

template<typename Container>
void printTable (Container const& table)
{
    for(auto it = table.begin(); it != table.end(); it++) {
        std::cout << "[";
        printRow(*it);
        std::cout << "]" << (it == table.end() - 1 ? "" : "\n") << std::endl;
    }
}

constexpr Indices makeRowIndices() {
    Indices out;

    for(int i = 0; i < 9; i++) {
        IndexRow row;
        for (int j = 0; j < 9; j++) {
            row[j] = (i + j*9);
        }

        out[i] = row;
    }

    return out;
}

std::ostream& operator<<(std::ostream& os, Cell const& cell)
{
    os << cell.toChar();
    return os;
}
