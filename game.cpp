
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include "game.h"
#include "Point.h"
#include "cursor.h"
#include "game.h"
//struct timeval tp;


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
pthread_mutex_t mutex_lock;


void *keyEvent(void *p) {
    Game *g = (Game *) p;
    char key;
    while (true) {
        key = getchar();
        switch (key) {
            case 'a':
                g->move(LEFT);
                break;
            case 'd':
                g->move(RIGHT);
                break;
            case 's':
                g->move(DOWN);
                break;
            case ' ':
                g->hard_drop();
                break;
            case 'w':
                g->SRS(true);
                break;
            case 'x':
                g->SRS(false);
                break;
            default:
                break;
        }
    }
}

Game::Game() : score(0), currentMino(nullptr), ghostMino(nullptr) {
    memset((void *) board, 0, (WIDTH + 2) * (HEIGHT + 2) * sizeof(int));
    memset((void *) color, 0, (WIDTH + 2) * (HEIGHT + 2) * 6);
    InitBoard();
}

Game::~Game() {

}

void Game::InitBoard() {
    for (int i = 0; i < HEIGHT + 2; i++) {
        board[i][0] = -1;
        board[i][11] = -1;
        color[i][0] = WHITE;
        color[i][11] = WHITE;

    }
    for (int j = 0; j < (WIDTH + 2); j++) {
        board[0][j] = -1;
        board[21][j] = -1;
        color[21][j] = WHITE;
        color[0][j] = WHITE;
    }
    Render();
    fflush(stdout);
}

void Game::printInstruction() {

}

void Game::initGame() {
    printf("\e[?25l");
    currentMinoBag = MinoBag();
    currentMino = currentMinoBag.getTopMino();
    currentMino->updateShape();
    currentMino->setCoord(1, 4);
}

void Game::move(int direction) {
    pthread_mutex_lock(&mutex_lock);
    switch (direction) {
        case LEFT:
            if (!checkBoundary(LEFT)) {
                currentMino->printMino(TRANS);
                currentMino->move(LEFT);
                currentMino->printMino(currentMino->getColor());
                ghost_drop();
            }
            break;
        case RIGHT:
            if (!checkBoundary(RIGHT)) {
                currentMino->printMino(TRANS);
                currentMino->move(RIGHT);
                currentMino->printMino(currentMino->getColor());
                ghost_drop();
            }
            break;
        case DOWN:
            if (!checkBoundary(DOWN)) {
                currentMino->printMino(TRANS);
                currentMino->move(DOWN);
                currentMino->printMino(currentMino->getColor());
                ghost_drop();
            } else {
                lockBlock();
                checkline();
                drawMino();
            }
            break;
        default:
            break;
    }

    Render();
    pthread_mutex_unlock(&mutex_lock);
}

bool Game::checkBoundary(int direction) {
    int shape[4][4] = {0};
    int x_coord, y_coord;
    memcpy(shape, currentMino->getShape(), 4 * 4 * sizeof(int));
    currentMino->getCoord(x_coord, y_coord);

    int x_range = 0, y_range = 0;
    (direction == DOWN) ? x_range++ : 0;
    (direction == LEFT) ? y_range-- : (direction == RIGHT) ? y_range++ : 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (shape[i][j] && board[i + x_coord + x_range][j + y_coord + y_range] == -1) {
                return true;
            }
        }
    }
    return false;
}

void Game::hard_drop() {
    while (!checkBoundary(DOWN)) {
        currentMino->printMino(TRANS);
        currentMino->move(DOWN);
        Render();
    }
    currentMino->printMino(currentMino->getColor());
}

void Game::ghost_drop() {
    if (ghostMino != nullptr)
        ghostMino->printGhostMino(TRANS);
    ghostMino = currentMino->clone();
    while (!checkBoundary(ghostMino, DOWN)) {
        ghostMino->move(DOWN);
        Render();
    }
    ghostMino->printGhostMino(ghostMino->getColor());
}

void Game::lockBlock() {
    int shape[4][4] = {0};
    int x_coord, y_coord;
    memcpy(shape, currentMino->getShape(), 4 * 4 * sizeof(int));
    currentMino->getCoord(x_coord, y_coord);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            board[x_coord + i][y_coord + j] -= shape[i][j];
            if (shape[i][j])
                color[x_coord + i][y_coord + j] = currentMino->getColor();
        }
    }

    currentMino = nullptr;
    getNext();

}

void Game::Render() {
    for (int i = 0; i < HEIGHT + 2; i++) {
        for (int j = 0; j < WIDTH + 2; j++) {
            if (board[i][j] == -1) {
                p.setCoordinate(i, j + 5);
                p.setColor(color[i][j]);
                p.printPoint(true);
            }
        }
    }
    fflush(stdout);
}

void Game::getNext() {
    if (currentMinoBag.getLeft() > 0)
        currentMinoBag.popMino();
    else
        currentMinoBag = MinoBag();
}

void Game::drawMino() {
    currentMino = currentMinoBag.getTopMino();
    currentMino->updateShape();
    currentMino->setCoord(1, 3);
}

int Game::checkline() {
    int tetris = 0;
    int chck = 0;
    for (int line = HEIGHT + 1; line > 0; line--) {
        for (int j = 1; j < WIDTH + 2; j++) {
            if (board[line][j])
                chck++;
        }
        if (chck == 10) {
            tetris++;
            pullboard(line);
            line++;
        }
    }
    return tetris;
}

void Game::pullboard(int count) {
    for (int i = count; i > 1; i--) {
        for (int j = 1; j < WIDTH + 1; j++) {
            board[i][j] = board[i - 1][j];
            color[i][j] = color[i - 1][j];
        }
    }
    Render();
}

void Game::SRS(bool clockwise) {
    int rotation_type = -1;
    int x_shift = 0, y_shift = 0;
    int temp[4][4]{0};
    bool flag = false;
    switch (currentMino->getState()) {
        case 0:
            rotation_type = clockwise ? 0 : 7;
            break;
        case 1:
            rotation_type = clockwise ? 2 : 1;
            break;
        case 2:
            rotation_type = clockwise ? 4 : 3;
            break;
        case 3:
            rotation_type = clockwise ? 6 : 5;
            break;
        default:
            break;
    }
    memcpy(temp, currentMino->temporaryRotation(clockwise), BLOCK_SIZE);
    currentMino->printMino(TRANS);
    if (currentMino->getType() != 'I') {
        for (int i = 0; i < 5; i++) {
            x_shift = -1 * SRSTest::WallKickDataJLSTZ[rotation_type][i][1];
            y_shift = SRSTest::WallKickDataJLSTZ[rotation_type][i][0];
            if (!checkcollision((int *) temp, x_shift, y_shift)) {
                flag = true;
                break;
            }

        }
    } else {
        for (int i = 0; i < 5; i++) {
            x_shift = -1 * SRSTest::WallKickDataI[rotation_type][i][1];
            y_shift = SRSTest::WallKickDataI[rotation_type][i][0];
            if (!checkcollision((int *) temp, x_shift, y_shift)) {
                flag = true;
                break;
            }
        }
    }
    if (flag) {
        currentMino->deltaX(x_shift);
        currentMino->deltaY(y_shift);
        currentMino->Rotation(clockwise);
        currentMino->updateShape();
    }

}

bool Game::checkcollision(int *matrix, int x_shift, int y_shift) {
    int shape[4][4] = {0};
    int x_coord, y_coord;
    memcpy(shape, matrix, 4 * 4 * sizeof(int));
    currentMino->getCoord(x_coord, y_coord);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (shape[i][j] && board[i + x_coord + x_shift][j + y_coord + y_shift] == -1) {
                return true;
            }
        }
    }
    return false;
}

void Game::GhostMode() {

}

bool Game::checkBoundary(Tetromino *t, int direction) {
    int shape[4][4] = {0};
    int x_coord, y_coord;
    memcpy(shape, t->getShape(), 4 * 4 * sizeof(int));
    t->getCoord(x_coord, y_coord);

    int x_range = 0, y_range = 0;
    (direction == DOWN) ? x_range++ : 0;
    (direction == LEFT) ? y_range-- : (direction == RIGHT) ? y_range++ : 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (shape[i][j] && board[i + x_coord + x_range][j + y_coord + y_range] == -1) {
                return true;
            }
        }
    }
    return false;
}


int main() {
//    signal(SIGINT, Signalhandler);
    pthread_t t1;
    pthread_mutex_init(&mutex_lock, nullptr);
    std::srand(std::time(0));
    system("clear");
    system("stty -icanon -echo");
    Game g;
    g.initGame();
    g.initGame();
    pthread_create(&t1, nullptr, keyEvent, &g);
    while (true) {
        fflush(stdout);
        g.move(DOWN);
        usleep(1500000);
    }
}

#pragma clang diagnostic pop