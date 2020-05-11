//
// Created by Yoon Jun on 2020/05/08.
//
//https://tetris.wiki/Super_Rotation_System
#ifndef ASSN3_1_TETROMINO_H
#define ASSN3_1_TETROMINO_H

#include "color.h"
#include "Point.h"
#include "data.h"
#include <iostream>

#define MOD(a, b) (b + (a%b)) % b
#define LEFT 0
#define RIGHT 1
#define DOWN 2


class Tetromino {
protected:
    int x, y;
    int state;
    int shape[4][4]{};
    std::string color;
    char type{};
public:
    Tetromino() {
        x = 0;
        y = 0;
        state = 0;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = 0;
    }

    ~Tetromino() = default;

    Tetromino(const Tetromino &mino) {
        x = mino.x;
        y = mino.y;
        state = mino.state;
        color = mino.color;
        type = mino.type;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = mino.shape[i][j];
    }

    int getX();

    int getY();

    void deltaX(int x_shift);

    void deltaY(int y_shift);

    int getState();

    virtual void setCoord(int x_, int y_);

    void defaultRotation(bool clockwise);

    void getCoord(int &x_, int &y_);

    void printMino(std::string color);

    void printGhostMino(std::string color);

    int *getShape();

    int move(int direction);

    std::string getColor();

    char getType();

    virtual void updateShape() = 0;

    virtual int *temporaryRotation(bool clockwise) = 0;

    void Rotation(bool clockwise);

    virtual Tetromino *clone() const = 0;


};

class Imino : public Tetromino {
public:
    Imino() {
        color = CYAN;
        type = 'I';
        memcpy(shape, RotationState::IRotationState[state], BLOCK_SIZE);
    }

    int *temporaryRotation(bool clockwise) override {
        return (int *) RotationState::IRotationState[MOD(state + (clockwise ? 1 : -1), 4)];
    }

    void updateShape() override;

    Imino *clone() const override {
        return new Imino(*this);
    }
};

class Omino : public Tetromino {
public:
    Omino() {
        color = YELLOW;
        type = 'O';
        memcpy(shape, RotationState::ORotationState[state], BLOCK_SIZE);

    }

    int *temporaryRotation(bool clockwise) override;

    void updateShape() override;

    Omino *clone() const override {
        return new Omino(*this);
    }
};

class Tmino : public Tetromino {
public:
    Tmino() {
        color = MAGENTA;
        type = 'T';
        memcpy(shape, RotationState::TRotationState[state], BLOCK_SIZE);

    }

    int *temporaryRotation(bool clockwise) override;

    void updateShape() override;

    Tmino *clone() const override {
        return new Tmino(*this);
    }
};

class Lmino : public Tetromino {
public:
    Lmino() {
        color = ORANGE;
        type = 'L';
        memcpy(shape, RotationState::LRotationState[state], BLOCK_SIZE);

    }

    int *temporaryRotation(bool clockwise) override;

    void updateShape() override;

    Lmino *clone() const override {
        return new Lmino(*this);
    }
};

class Jmino : public Tetromino {
public:
    Jmino() {
        color = BLUE;
        type = 'J';
        memcpy(shape, RotationState::JRotationState[state], BLOCK_SIZE);

    }

    int *temporaryRotation(bool clockwise) override;

    void updateShape() override;

    Jmino *clone() const override {
        return new Jmino(*this);
    }

};

class Smino : public Tetromino {
public:
    Smino() {
        color = GREEN;
        type = 'S';
        memcpy(shape, RotationState::SRotationState[state], BLOCK_SIZE);

    }

    int *temporaryRotation(bool clockwise) override;

    void updateShape() override;

    Smino *clone() const override {
        return new Smino(*this);
    }

};

class Zmino : public Tetromino {
public:
    Zmino() {
        color = RED;
        type = 'Z';
        memcpy(shape, RotationState::ZRotationState[state], BLOCK_SIZE);

    }

    int *temporaryRotation(bool clockwise) override {
        return (int *) RotationState::ZRotationState[MOD(state + (clockwise ? 1 : -1), 4)];
    }

    void updateShape() override {
        memcpy(shape, RotationState::ZRotationState[state], BLOCK_SIZE);
    }

    Zmino *clone() const override {
        return new Zmino(*this);
    }
};

class MinoBag {
private:
    Tetromino *top;
    int seqence[7];
    Tetromino *minoList[7];
    int left;
public:
    MinoBag() {
        left = 7;
        int idx = 0;
        Tetromino *_tetromino = nullptr;
        randMino(seqence);
        for (int i: seqence) {
            switch (i) {
                case 1:
                    minoList[idx++] = new Imino();
                    break;
                case 2:
                    minoList[idx++] = new Omino();
                    break;
                case 3:
                    minoList[idx++] = new Tmino();
                    break;
                case 4:
                    minoList[idx++] = new Lmino();
                    break;
                case 5:
                    minoList[idx++] = new Jmino();
                    break;
                case 6:
                    minoList[idx++] = new Smino();
                    break;
                case 0:
                    minoList[idx++] = new Zmino();
                    break;
                default:
                    break;
            }
        }
        popMino();
    }

    ~MinoBag() {
    }

    void popMino() {
        top = minoList[--left];
    }

    int getLeft() {
        return left;
    }

    Tetromino *getTopMino() {
        return top;
    }

    std::string getColor() {
        return top->getColor();
    }

    Tetromino **getMinoBag() {
        return minoList;
    }


    void clearMino() {
        top->printMino(TRANS);
    }

    void randMino(int *seq) {
        int temp[7]{0};
        int idx = 0;
        int r;
        while (idx < 7) {
            r = std::rand() % 7;
            if (temp[r] == 0) {
                temp[r] = 1;
                seq[idx++] = r;
            }
        }
    }

};

#endif //ASSN3_1_TETROMINO_H
