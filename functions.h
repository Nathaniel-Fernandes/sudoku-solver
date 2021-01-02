#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include "functions.h"

#define expect_eq(expected, actual) cout << line << endl;\
    if (expected != actual) {\
        cout << "[FAIL]" << endl;\
        cout << " expected = " << expected << endl;\
        cout << " actual = " << actual << endl;\
    } else {\
        cout << "[PASS]" << endl;\
    }\

struct Board {
    int board[9][9] {0};
};

struct Boards_Array {
    Board* next = nullptr;
    int size = 0;
    int index = 0;
};

Board operator<<(std::ostream& os, Board& b);

#endif