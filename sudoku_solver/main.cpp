#include <algorithm> // use -std=c++20 for std::ranges
#include <iostream>
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


struct Cell {
    Cell(){}
    explicit Cell(int i) : vals({i}){}
    std::unordered_set<int> vals = {1, 2, 3, 4, 5, 6, 7, 8, 9};
};

using Board = std::vector<std::vector<Cell>>;
namespace ranges = std::ranges;

Board makeBoard(std::vector<std::vector<char>> const& input) {
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

void printRow(std::vector<Cell> const& row)
{
    for(auto it = row.begin(); it != row.end(); it++) {
        Cell const& cell = *it;
        std::cout
            << (cell.vals.size() > 1 ? '.' : (char) (*cell.vals.begin() + 48)) << (it == row.end() - 1 ? "" : ", ");
    }
}

void printBoard (Board const& brd)
{
    for(auto it = brd.begin(); it != brd.end(); it++) {
        std::cout << "[";
        printRow(*it);
        std::cout << "]" << (it == brd.end() - 1 ? "" : "\n") << std::endl;
    }
}

int main() {
    Board board = makeBoard(INPUT);

    printBoard(board);
}
