#include "grid.h"
#include <stdlib.h>

void createSnakeTile(SnakeTile* head, Grid* g){
	if(head->next != NULL){
		createSnakeTile(head->next, g);//Creates the new tile as the last item
		return;
	}

	SnakeTile* tile = (SnakeTile*) malloc(sizeof(SnakeTile));
	tile->direction = head->direction;
	tile->next = NULL;
	//The tile should be positioned "opposite" to the direction of previous tile
	switch (head->direction){
	case UP:
		tile->pos.x = head->pos.x;
		tile->pos.y = head->pos.y + 1;
		break;
	case RIGHT:
		tile->pos.x = head->pos.x - 1;
		tile->pos.y = head->pos.y;
	case DOWN:
		tile->pos.x = head->pos.x;
		tile->pos.y = head->pos.y -1;
	case LEFT:
		tile->pos.x = head->pos.x + 1;
		tile->pos.y = head->pos.y;
	default:
		break;
	}
	
	head->next = tile;
	g->playfield[tile->pos.x][tile->pos.y] = SNAKE;
}

SnakeTile* createHead(Grid* g){
	SnakeTile *head = (SnakeTile*) malloc(sizeof(SnakeTile));
	u8 initialX = g->width/2;
	u8 initialY = g->height/2;

	head->direction = RIGHT;
	head->next = NULL;
	
	head->pos.x = initialX;
	head->pos.y = initialY;

	g->playfield[head->pos.x][head->pos.y] = SNAKE;

	return head;
}

void updateSnakeMoveDir(SnakeTile* head, Directions direction){
	if(head->next != NULL){
		updateSnakeMoveDir(head->next, head->direction);//The next tile moves in the same direction of the previous tile
	}
	head->direction = direction;
}
void moveSnake(SnakeTile* head, Grid* g, bool* gameOver){
	Coord previousPos = {head->pos.x, head->pos.y};
	switch (head->direction){
		case RIGHT:
			head->pos.x += 1;
			break;
		case DOWN:
			head->pos.y += 1;
			break;
		case LEFT:
			head->pos.x -= 1;
			break;
		case UP:
			head->pos.y -= 1;
			break;
		default:
			break;
	}

	if(g->playfield[head->pos.x][head->pos.y] == SNAKE ||
		(head->pos.y >= g->height || head->pos.y < 0) ||
		(head->pos.x >= g->width || head->pos.x < 0)){
			head->pos.x = previousPos.x;
			head->pos.y = previousPos.y;
			*gameOver = 1;
			return;
	}

	//Tile has food
	if(g->playfield[head->pos.x][head->pos.y] == FOOD){
		createSnakeTile(head, g);
	}

	g->playfield[head->pos.x][head->pos.y] = SNAKE;

	g->playfield[previousPos.x][previousPos.y] = EMPTY;
	if(head->next != NULL) moveSnake(head->next, g, gameOver);
}


void deallocSnake(SnakeTile *head){
	if(head->next != NULL){
		deallocSnake(head->next);
	}
	free(head);
}