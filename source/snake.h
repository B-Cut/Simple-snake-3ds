#ifndef SNAKE_H
#define SNAKE_H

#include "util.h"
#include <3ds.h>


SnakeTile* createHead();
void createSnakeTile(SnakeTile* head);
//void updateSnakeMoveDir(SnakeTile* head, Directions direction);
void moveSnake(SnakeTile* head, bool* gameOver, Directions direction);
u8 checkCollision(SnakeTile* head, Coord* food);
void deallocSnake(SnakeTile* head);

#endif