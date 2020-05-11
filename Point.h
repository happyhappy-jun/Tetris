//
// Created by Yoon Jun on 2020/05/07.
//

#ifndef ASSN3_1_POINT_H
#define ASSN3_1_POINT_H

#include <cstring>
#include <iostream>
#include "color.h"
#include "cursor.h"

class Point {
protected:
    int x, y;
    std::string color;
public:
    Point() {
        color = TRANS;
        x = 0, y = 0;
    };

    Point(std::string color, int x, int y) {
        this->color = color;
        this->x = x;
        this->y = y;
    }

    void setCoordinate(const int x, const int y) {
        printf("\033[%d;%dH", (x), (y));
    }

    void setColor(std::string c) {
        this->color = c;
    }

    std::string getColor() {
        return color;
    }

    void printPoint(bool filled);

    void printString(std::string s);
};


#endif //ASSN3_1_POINT_H
