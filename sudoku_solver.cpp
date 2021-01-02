#include <iostream>
#include <ostream>
#include <string>
#include <fstream>
#include <chrono>
#include "functions.h"

using std::cin, std::cout, std::endl;
using std::ostream, std::ifstream;
using std::string;
using std::chrono::high_resolution_clock, std::chrono::duration_cast, std::chrono::microseconds;

Board operator<<(ostream& os, Board& b);
Board* solve_board(Board& b);
Boards_Array* possibilities(Board& b);
Board* backtrack(Boards_Array* ba);
void copy_board(Board& original, Board& new_one);

bool is_valid(Board& b);
bool solved(Board& b);
bool rows_good(Board& b);
bool cols_good(Board& b);
bool squares_good(Board& b);


int main() {
    // open board input
    string filename;
    cout << "Please enter file name: ";
    cin >> filename;

    ifstream ifs(filename);

    if(!ifs.is_open()) {
        cout << "Sorry! Failed to open " << filename << endl;
        return -1;
    }

    auto start = high_resolution_clock::now();

    Board x;
    int input;
    int row = 0;
    int col = 0;

    while(ifs >> input) {
        x.board[row][col] = input;

        if (col < 8) col++;
        else {
            row++;
            col = 0;
        }
    }

    // cout << x;

    // Solve board
    solve_board(x);
    // possibilities(x);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start); 

    cout << "This function took: " << duration.count() << " microseconds to run" << endl;

    return 0;
}


Board* backtrack(Boards_Array* ba) {
    if (ba->size == 0 || ba->index == ba->size) {
        // return bad borad - to do
        // cout << "backtrack delete";
        delete[] ba->next;
        ba->next = nullptr;
        delete[] ba;
        ba = nullptr;
        return nullptr;
    } 

    else {
        // do backtrack search on board
        // cout << ba.next[ba.index];
        Board* test_board = solve_board(ba->next[ba->index]);

        if(test_board != nullptr) {
            // cout << test_board;
            delete[] ba->next;
            ba->next = nullptr;
            delete[] ba;
            ba = nullptr;
            return test_board;
        } else {
            ba->index++;
            return backtrack(ba);
        }
    }
}

Board* solve_board(Board& b) {
    if(solved(b)) {
        cout << b;
        return &b;
    }

    else {
        // do backtrack search
        // cout << b;
        Boards_Array* possible = possibilities(b);  
        return backtrack(possible);
    }
}

Boards_Array* possibilities(Board& b) {
    // cout << b;
    // returns pointer to a new array that contains next possible boards
    int empty_row = -1;
    int empty_col = -1;

    for(int row = 0; row < 9; row++) {
        for(int col = 0; col < 9; col++) {
            if(b.board[row][col] == 0) {
                empty_row = row;
                empty_col = col;
                break;
            }
        }
        if(empty_row != -1) break;
    }
    // cout << "empty row " << empty_row << endl;
    // cout << "empty col " << empty_col << endl;

    int valid_possibilities[9] {0};
    int count = 0;

    if (empty_row != -1) {
        for(int i = 1; i < 10; i++) {
            b.board[empty_row][empty_col] = i;

            // cout << b;

            if(is_valid(b)) {
                // cout << "is valid!" << endl;
                valid_possibilities[count] = i;
                count++;
            }
        }
    }
    // cout << "count: " << count;
    // create a pointer to new array of boards of length count
    Boards_Array* next_possibilities = new Boards_Array;
    (*next_possibilities).next = new Board[count];
    (*next_possibilities).size = count;

    // copy position from original board to new one
    // replace 
    for(int i = 0; i < count; i++) {
        copy_board(b, (*next_possibilities).next[i]);
        (*next_possibilities).next[i].board[empty_row][empty_col] = valid_possibilities[i];
    }
    
    // cout << "Hi" << endl;
    return next_possibilities;
}

void copy_board(Board& original, Board& new_one) {
    for(int row = 0; row < 9; row++) {
        for(int col = 0; col < 9; col++) {
            new_one.board[row][col] = original.board[row][col];
        }
    }
}

Board operator<<(ostream& os, Board& b) {
    os << " - - - - - - - - - - - -" << endl;

    for(int row = 0; row < 9; row++) {
        os << "|";
        for(int col = 0; col < 9; col++) {
            if(b.board[row][col] == 0) {
                os << " :";
            }
            else {
                os << " " << b.board[row][col];
            }

            if ((col - 2) % 3 == 0) {
                os << " |";
            }
        }
        os << endl;
        if ((row - 2) % 3 == 0) {
            os << " - - - - - - - - - - - -" << endl;
        }
    }
}

bool is_valid(Board& b) {
    return rows_good(b) && cols_good(b) && squares_good(b);
}

bool rows_good(Board& b) {
    int values[9] {0};
    int count = 0;

    int cell_value = 0;

    for(int row = 0; row < 9; row++) {
        for(int col = 0; col < 9; col++) {
            cell_value = b.board[row][col];

            for(int i = 0; i < count; i++) {

                if(cell_value != 0 && cell_value == values[i]) {
                    // cout << "failed rows: " << row << " " << col << endl;
                    return false;
                }
                // cout << "hi there" << endl;
            }

            // add value to values
            if(cell_value != 0) {
                // cout << "suppp";
                values[count] = cell_value;
                count++;
            }
        }

        // reset values[] and count for next row
        for(int i = 0; i < count; i++) {
            values[i] = 0;
        }
        count = 0;
    }

    // cout << "what's good" << endl;

    return true;
}

bool cols_good(Board& b) {
    int values[9] {0};
    int count = 0;

    int cell_value;

    for(int col = 0; col < 9; col++) {
        for(int row = 0; row < 9; row++) {
            cell_value = b.board[row][col];

            for(int i = 0; i < count; i++) {
                cell_value = b.board[row][col];

                if(cell_value == values[i]) {
                    // cout << "failed cols: " << row << " " << col << endl;
                    return false;
                }
            }

            if(cell_value != 0) {
                // cout << "sup";
                values[count] = cell_value;
                count++;
            }
        }
        // reset values and count for next column
        for(int i = 0; i < count; i++) {
            values[i] = 0;
        }
        count = 0;
    }

    return true;
}

bool squares_good(Board& b) {
    int values[9] {0};
    int count = 0;

    int row_coord, col_coord, cell_value;

    for(int h_offset = 0; h_offset < 3; h_offset++) {
        for(int v_offset = 0; v_offset < 3; v_offset++) {
            for(int col = 0; col < 3; col++) {
                for(int row = 0; row < 3; row++) {
                    row_coord = row + 3*v_offset;
                    col_coord = col + 3*h_offset;

                    cell_value = b.board[row_coord][col_coord];

                    for(int i = 0; i < count; i++) {
                        if(cell_value != 0 && cell_value == values[i]) {
                            // cout << "failed squares: " << row << " " << col << endl;

                            return false;
                        }
                    }

                    if(cell_value != 0) {
                        values[count] = cell_value;
                        count++;
                    }
                }
            }
            // reset values[] and count
            for(int i = 0; i < count; i++) {
                values[i] = 0;
            }
            count = 0;
        }
    }
    return true;
}

bool solved(Board& b) {
    // board is tautologically solved if a] it's valid b] every square is filled 
    if (!is_valid(b)) {
        return false;
    }

    for(int row = 0; row < 9; row++) {
        for(int col = 0; col < 9; col++) {
            if(b.board[row][col] == 0) {
                return false;
            }
        }
    }

    return true;
}