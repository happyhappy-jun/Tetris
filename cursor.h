//
// Created by Yoon Jun on 2020/05/07.
//

#ifndef ASSN3_1_CURSOR_H
#define ASSN3_1_CURSOR_H

#include <iostream>

class cursor {
public:
    void saveCursor();
    void moveCursor(const int x, const int y);
    void restoreCursor();
};


#endif //ASSN3_1_CURSER_H
