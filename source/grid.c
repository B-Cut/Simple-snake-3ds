#include "grid.h"
#include <stdlib.h>

Grid* initGrid(u8 width, u8 height){
	Grid* g = (Grid*) malloc(sizeof(Grid));
	
	g->height = height;
	g->width = width;
	
    //Creates an empty grid
	TileType** field = (TileType**) malloc(sizeof(TileType*) * height);
	for(u8 i = 0; i < height; i++){
		field[i] = (TileType*) malloc(sizeof(TileType) * width);
        for(int j = 0; j < width; j++) field[i][j] = EMPTY;
	}

	g->playfield = field;
	return g;
}
void deallocGrid(Grid* g){
	for(u8 i = 0; i < g->height; i++){
		free(g->playfield[i]);
	}
	free(g->playfield);
	free(g);
}