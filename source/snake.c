#include "util.h"
#include <stdlib.h>

SnakeTile* createSnakeTile(SnakeTile* tail){
	SnakeTile* tile = (SnakeTile*) malloc(sizeof(SnakeTile));
	tile->direction = tail->direction;
	tile->next = NULL;
	//The tile should be positioned "opposite" to the direction of previous tile
	switch (tail->direction){
	case UP:
		tile->pos.x = tail->pos.x;
		tile->pos.y = tail->pos.y + 1;
		break;
	case RIGHT:
		tile->pos.x = tail->pos.x - 1;
		tile->pos.y = tail->pos.y;
	case DOWN:
		tile->pos.x = tail->pos.x;
		tile->pos.y = tail->pos.y -1;
	case LEFT:
		tile->pos.x = tail->pos.x + 1;
		tile->pos.y = tail->pos.y;
	default:
		break;
	}
	
	tail->next = tile;
	return tile;
}

SnakeTile* createHead(){
	SnakeTile *head = (SnakeTile*) malloc(sizeof(SnakeTile));

	head->direction = RIGHT;
	head->next = NULL;
	
	head->pos.x = GRID_WIDTH/2;
	head->pos.y = GRID_HEIGHT/2;

	return head;
}

/*void updateSnakeMoveDir(SnakeTile* head, Directions direction){
	if(head->next != NULL){
		updateSnakeMoveDir(head->next, head->direction);//The next tile moves in the same direction of the previous tile
	}

	head->direction = direction;
	
}*/

void moveSnake(SnakeTile* head, bool* gameOver, Directions direction){
	Coord previousPos = head->pos;
	Directions previousDirection = head->direction;
	switch (direction){
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
	head->direction = direction;

	for(SnakeTile* tile = head->next; tile != NULL; tile = tile->next){
		Coord tempPos = tile->pos;
		tile->pos = previousPos;
		previousPos = tempPos;

		Directions tempDir = tile->direction;
		tile->direction = previousDirection;
		previousDirection = tempDir;
	}
}


void deallocSnake(SnakeTile *head){
	if(head->next != NULL){
		deallocSnake(head->next);
	}
	free(head);
}

u8 checkCollision(SnakeTile* head, Coord* food){
	//Returns 1 if snake collides with itself or the walls, 2 if it collides with food and 0 if it doesn't collide

	if(head->pos.x >= GRID_WIDTH || head->pos.x < 0) return 1;
	if(head->pos.y >= GRID_HEIGHT || head->pos.y < 0) return 1;

	for(SnakeTile* tile = head->next; tile != NULL; tile = tile->next){
		if (tile->pos.x == head->pos.x && tile->pos.y == head->pos.y){
			//checks if head overlaps with other part of the body
			return 1;
		}	
	}

	if(head->pos.x == food->x && head->pos.y == food->y) return 2;

	return 0;
}