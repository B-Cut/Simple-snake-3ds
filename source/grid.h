#ifndef UTIL_H
#define UTIL_H
#include <citro2d.h>

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

#define TILE_WIDTH 10
#define TILE_HEIGHT 10

#define GRID_WIDTH 40
#define GRID_HEIGHT 24

typedef struct grid{
	u8 width;
	u8 height;

	TileType** playfield;
} Grid;

Grid* initGrid(u8 width, u8 height);
void deallocGrid(TileType** grid, u8 width, u8 height);

typedef enum directions{RIGHT, DOWN, LEFT, UP} Directions;
typedef enum tiletype{EMPTY, SNAKE, FOOD} TileType;

typedef struct coord{
	u8 x;
	u8 y;
} Coord;

typedef struct snake_tile{
	Coord pos;
	Directions direction;
	struct snake_tile *next;
} SnakeTile;



#endif