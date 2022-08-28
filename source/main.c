//TODO: Implement a states system

#include <citro2d.h>
#include <3ds.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "snake.h"
#include "grid.h"



//Playfield code


//Food code
void spawnFood(TileType** grid);

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
	
	Grid* g = initGrid(GRID_WIDTH, GRID_HEIGHT);
	SnakeTile* head = createHead(g);

	Directions currentMoveDirection = RIGHT;

	bool gameOver = 0;

	// Main loop
	while (aptMainLoop())
	{
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
		moveSnake(head, g, &gameOver);
		
		

		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, clrClear);
		C2D_SceneBegin(top);

		for(u8 i = 0; i < GRID_WIDTH; i++){
			for(u8 j = 0; j < GRID_HEIGHT; j++){
				switch (g->playfield[i][j])
				{
				case EMPTY:
					C2D_DrawRectSolid(i*TILE_WIDTH, j*TILE_HEIGHT, 0, TILE_WIDTH, TILE_HEIGHT, clrBackground);
					break;
				case FOOD:
					C2D_DrawRectSolid(i*TILE_WIDTH, j*TILE_HEIGHT, 0, TILE_WIDTH, TILE_HEIGHT, clrFood);
					break;
				case SNAKE:
					C2D_DrawRectSolid(i*TILE_WIDTH, j*TILE_HEIGHT, 0, TILE_WIDTH, TILE_HEIGHT, clrSnake);
					break;
				default:
					break;
				}
			}
		}
		//TODO: look for a better way to control the game speed
		svcSleepThread(100000000);
		C2D_Flush();
		C3D_FrameEnd(0);
		
	}

	//Dealloc elements
	deallocGrid(g);
	deallocSnake(head);

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
}


void spawnFood(TileType** grid){
	srand(osGetTime());
	u8 posX = rand() % GRID_WIDTH;
	u8 posY = rand() % GRID_HEIGHT;

	if(grid[posX][posY] == EMPTY){
		grid[posX][posY] = FOOD;
		return;
	}
	else spawnFood(grid);
	
}