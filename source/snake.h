#ifndef SNAKE_H
#define SNAKE_H

#include "grid.h"



SnakeTile* createHead(Grid* g);
void createSnakeTile(SnakeTile* head, Grid* g);
void updateSnakeMoveDir(SnakeTile* head, Directions direction);
void moveSnake(SnakeTile* head, Grid* g, bool* gameOver);
void deallocSnake(SnakeTile* head);

#endif