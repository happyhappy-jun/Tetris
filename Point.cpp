//
// Created by Yoon Jun on 2020/05/07.
//

#include "Point.h"
#include <cstdio>
#include <iostream>
#include <string>

void Point::printPoint(bool filled) {
    cursor c;
    c.saveCursor();
    c.moveCursor(x,y);
    std::cout << RST << color << (filled?FILL:" ");
    c.restoreCursor();
}

void Point::printString(std::string s){
    cursor c;
    c.saveCursor();
    c.moveCursor(x,y);
    std::cout << RST << color << s;
    c.restoreCursor();
}