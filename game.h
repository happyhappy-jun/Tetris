//
// Created by Yoon Jun on 2020/05/07.
//

#ifndef ASSN3_1_GAME_H
#define ASSN3_1_GAME_H

#include "Point.h"
#include "Tetromino.h"

#define HEIGHT 20
#define WIDTH 10
#define LEFT 0
#define RIGHT 1
#define DOWN 2
#define FALL 3

void Signalhandler(int s);

void *keyEvent(void *p);

class Game {
private:
    int board[HEIGHT + 2][WIDTH + 2]{};
    std::string color[HEIGHT + 2][WIDTH + 2]{};
    int score;
    Point p;

    MinoBag currentMinoBag;
    Tetromino *currentMino;
    Tetromino *ghostMino;
public:
    Game();

    ~Game();

    void initGame();

    void printInstruction();

    void move(int direction);

    bool checkcollision(int *matrix, int x_shift, int y_shift);

    void SRS(bool clockwise);

    void hard_drop();

    void ghost_drop();

    bool checkBoundary(int direction);

    bool checkBoundary(Tetromino* t, int direction);

    int checkline();

    void InitBoard();

    void lockBlock();

    void pullboard(int count);

    void drawMino();

    void getNext();

    void Render();

    void GhostMode();

};

#endif //ASSN3_1_GAME_H
