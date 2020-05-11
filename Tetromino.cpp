//
// Created by Yoon Jun on 2020/05/08.
//

#include "Tetromino.h"
#include "Point.h"

int Tetromino::getX() {
    return x;
}

int Tetromino::getY() {
    return y;
}


void Tetromino::setCoord(int x_, int y_) {
    x = x_;
    y = y_;
}

void Tetromino::getCoord(int &x_, int &y_) {
    x_ = x;
    y_ = y;
}


int *Tetromino::getShape() {
    return (int *) shape;
}

int Tetromino::move(int direction) {
    switch (direction) {
        case LEFT:
            y--;
            break;
        case RIGHT:
            y++;
            break;
        case DOWN:
            x++;
            break;
    }
    return 0;
}

void Tetromino::printMino(std::string color) {
    Point p;
    for (int m = x; m < x + 4; m++) {
        for (int n = y; n < y + 4; n++) {
            if (shape[m - x][n - y]) {
                p.setCoordinate(m, 5 + n);
                p.setColor(color);
                p.printPoint(color != TRANS);
            }
        }
    }
}

void Tetromino::printGhostMino(std::string color) {
    Point p;
    for (int m = x; m < x + 4; m++) {
        for (int n = y; n < y + 4; n++) {
            if (shape[m - x][n - y]) {
                p.setCoordinate(m, 5 + n);
                p.setColor(color);
                if(color == TRANS)
                    p.printString(" ");
                else
                    p.printString(BLANK);
            }
        }
    }
}

std::string Tetromino::getColor() {
    return color;
}

int Tetromino::getState() {
    return state;
}

void Tetromino::defaultRotation(bool clockwise) {
    state += clockwise ? 1 : -1;
    state = state % 4;
}

char Tetromino::getType() {
    return type;
}

void Tetromino::deltaX(int x_shift) {
    x += x_shift;
}

void Tetromino::deltaY(int y_shift) {
    y += y_shift;
}

void Tetromino::Rotation(bool clockwise) {
    state = MOD((state + (clockwise?1:-1)),4);
}

void Tetromino::updateShape() {

}


void Imino::updateShape() {
    memcpy(shape, RotationState::IRotationState[state], BLOCK_SIZE);
}

int *Omino::temporaryRotation(bool clockwise) {
    return (int *) RotationState::ORotationState[MOD((state + (clockwise ? 1 : -1)), 4)];
}

void Omino::updateShape() {
    memcpy(shape, RotationState::ORotationState[state], BLOCK_SIZE);
}

int *Tmino::temporaryRotation(bool clockwise) {
    return (int *) RotationState::TRotationState[MOD(state + (clockwise ? 1 : -1), 4)];
}

void Tmino::updateShape() {
    memcpy(shape, RotationState::TRotationState[state], BLOCK_SIZE);
}

int *Lmino::temporaryRotation(bool clockwise) {
    return (int *) RotationState::LRotationState[MOD(state + (clockwise ? 1 : -1), 4)];
}

void Lmino::updateShape() {
    memcpy(shape, RotationState::LRotationState[state], BLOCK_SIZE);
}

int *Jmino::temporaryRotation(bool clockwise) {
    return (int *) RotationState::JRotationState[MOD(state + (clockwise ? 1 : -1), 4)];
}

void Jmino::updateShape() {
    memcpy(shape, RotationState::JRotationState[state], BLOCK_SIZE);
}

int *Smino::temporaryRotation(bool clockwise) {
    return (int *) RotationState::SRotationState[MOD(state + (clockwise ? 1 : -1), 4)];
}

void Smino::updateShape() {
    memcpy(shape, RotationState::SRotationState[state], BLOCK_SIZE);
}
