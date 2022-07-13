//TODO: Separate different functions in different files
//TODO: Implement a states system

#include <citro2d.h>
#include <3ds.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

#define TILE_WIDTH 10
#define TILE_HEIGHT 10

#define GRID_WIDTH 40
#define GRID_HEIGHT 24

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


//Snake Code
SnakeTile* createHead(u8 initialX, u8 initialY, TileType** grid);
void createSnakeTile(SnakeTile* head, TileType** grid);
void updateSnakeMoveDir(SnakeTile* head, Directions direction);
void moveSnake(SnakeTile* head, TileType** grid, bool* gameOver);
void deallocSnake(SnakeTile* head);

//Playfield code
TileType** initGrid(u8 width, u8 height);
void deallocGrid(TileType** grid, u8 width, u8 height);

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
	
	TileType** grid = initGrid(GRID_WIDTH, GRID_HEIGHT);
	SnakeTile* head = createHead(GRID_WIDTH/2, GRID_HEIGHT/2, grid);

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
			moveSnake(head, grid, &gameOver);
		
		

		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, clrClear);
		C2D_SceneBegin(top);

		for(u8 i = 0; i < GRID_WIDTH; i++){
			for(u8 j = 0; j < GRID_HEIGHT; j++){
				switch (grid[i][j])
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
	deallocGrid(grid, GRID_WIDTH, GRID_HEIGHT);
	deallocSnake(head);

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
}

void createSnakeTile(SnakeTile* head, TileType** grid){
	if(head->next != NULL){
		createSnakeTile(head->next, grid);//Creates the new tile as the last item
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
	grid[tile->pos.x][tile->pos.y] = SNAKE;
}

SnakeTile* createHead(u8 initialX, u8 initialY, TileType** grid){
	SnakeTile *head = (SnakeTile*) malloc(sizeof(SnakeTile));

	head->direction = RIGHT;
	head->next = NULL;
	
	head->pos.x = initialX;
	head->pos.y = initialY;

	grid[head->pos.x][head->pos.y] = SNAKE;

	return head;
}

void updateSnakeMoveDir(SnakeTile* head, Directions direction){
	if(head->next != NULL){
		updateSnakeMoveDir(head->next, head->direction);//The next tile moves as the previous tile
	}
	head->direction = direction;
}
void moveSnake(SnakeTile* head, TileType** grid, bool* gameOver){
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

	if(grid[head->pos.x][head->pos.y] == SNAKE ||(head->pos.y >= GRID_HEIGHT || head->pos.y < 0) || (head->pos.x >= GRID_HEIGHT || head->pos.x < 0)){
		head->pos.x = previousPos.x;
		head->pos.y = previousPos.y;
		*gameOver = 1;
		return;
	}

	//Tile has food
	if(grid[head->pos.x][head->pos.y] == FOOD){
		createSnakeTile(head, grid);
	}

	grid[head->pos.x][head->pos.y] = SNAKE;

	grid[previousPos.x][previousPos.y] = EMPTY;
	if(head->next != NULL) moveSnake(head->next, grid, gameOver);
}


void deallocSnake(SnakeTile *head){
	if(head->next != NULL){
		deallocSnake(head->next);
	}
	free(head);
}

TileType** initGrid(u8 width, u8 height){
	TileType** grid = (TileType**) malloc(sizeof(TileType*) * width);
	for(u8 i = 0; i < width; i++){
		grid[i] = (TileType*) malloc(sizeof(TileType) * width);
	}
	return grid;
}
void deallocGrid(TileType** grid, u8 width, u8 height){
	for(u8 i = 0; i < width; i++){
		free(grid[i]);
	}
	free(grid);
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