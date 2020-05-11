//
// Created by Yoon Jun on 2020/05/07.
//

#include "cursor.h"
#include <cstdio>

void cursor::saveCursor() {
    printf("\33[s");
}

void cursor::moveCursor(const int x, const int y) {
    int i;
    for(i = 0; i < y; i++)
        printf("\33[2C");
    for(i = 0; i < x; i++)
        printf("\33[1B");
}

void cursor::restoreCursor() {
    printf("\33[u");
}
