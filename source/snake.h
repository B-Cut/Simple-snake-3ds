#ifndef SNAKE_H
#define SNAKE_H

#include "grid.h"



SnakeTile* createHead(u8 initialX, u8 initialY, TileType** grid);
void createSnakeTile(SnakeTile* head, TileType** grid);
void updateSnakeMoveDir(SnakeTile* head, Directions direction);
void moveSnake(SnakeTile* head, TileType** grid, bool* gameOver);
void deallocSnake(SnakeTile* head);

#endif