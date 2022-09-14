//TODO: Implement a states system

#include <citro2d.h>
#include <3ds.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "snake.h"
#include "util.h"



//Playfield code


//Food code
void spawnFood(SnakeTile* head, Coord* food);

void doNothing(){}

//---------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//---------------------------------------------------------------------------------
	// Init libs
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM, NULL);

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	// Create colors
	u32 clrBackground  = C2D_Color32(0x9F, 0xF4, 0xE5, 0xFF);
	u32 clrSnake = C2D_Color32(0x00, 0x5F, 0x8C, 0xFF);
	u32 clrFood = C2D_Color32(0x00, 0x2B, 0x59, 0xFF);
	

	u32 clrClear = C2D_Color32(0xFF, 0xD8, 0xB0, 0x68);

	//define the number of tiles of the grid
	
	SnakeTile* head = createHead();
	Coord food = {0, 0};
	spawnFood(head, &food); 

	Directions currentMoveDirection = RIGHT;

	bool gameOver = 0;

	// Main loop
	while (aptMainLoop())
	{
		if(gameOver) continue; //Freezes the game on game over
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu
		printf("\x1b[1;1HCitro2D snake by BCut");
		printf("\x1b[1;1HUse the D-Pad to play");
		printf("\x1b[2;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
		printf("\x1b[3;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
		printf("\x1b[4;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);

		
		
		if(kDown & KEY_DRIGHT) currentMoveDirection = RIGHT;
		else if(kDown & KEY_DDOWN) currentMoveDirection = DOWN;
		else if(kDown & KEY_DLEFT) currentMoveDirection = LEFT;
		else if(kDown & KEY_DUP) currentMoveDirection = UP;

		updateSnakeMoveDir(head, currentMoveDirection);
		moveSnake(head, &gameOver);
		switch (checkCollision(head, &food)){
			case 1:
				gameOver = true;
				break;
			case 2:
				createSnakeTile(head);
				spawnFood(head, &food);
				break;
			default:
				break;
		}
		

		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, clrClear);
		C2D_SceneBegin(top);

		//Draw Food position
		C2D_DrawRectSolid(food.x * TILE_WIDTH, food.y * TILE_HEIGHT, 0, TILE_WIDTH, TILE_HEIGHT, clrFood);

		//There's no actual need for a grid to keep track of the snake position, we can just go down the linked list
		for(SnakeTile* tile = head; tile != NULL; tile = tile->next){
			C2D_DrawRectSolid(tile->pos.x * TILE_WIDTH, tile->pos.y * TILE_HEIGHT, 0, TILE_WIDTH, TILE_HEIGHT, clrSnake);
		}

		

		//TODO: look for a better way to control the game speed
		svcSleepThread(100000000);
		C2D_Flush();
		C3D_FrameEnd(0);
		
	}

	//Dealloc elements
	deallocSnake(head);

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
}


void spawnFood(SnakeTile* head, Coord* food){
	srand(osGetTime());
	food->x = rand() % GRID_WIDTH;
	food->y = rand() % GRID_HEIGHT;

	for(SnakeTile* tile = head; tile != NULL; tile = tile->next){
		if(tile->pos.x == food->x && tile->pos.y == food->y){
			return spawnFood(head, food); //repeats the function to generate new position
		}
	}
}