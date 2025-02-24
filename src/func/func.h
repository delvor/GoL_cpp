#ifndef FUNC_H
#define FUNC_H

#include "../ui/Button.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <vector>


// Window and grid settings
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int CELL_SIZE = 1;
extern int GRID_WIDTH;
extern int GRID_HEIGHT;

// Game state variables
extern bool running ;
extern bool paused ;
extern std::vector<std::vector<bool>> grid;
extern std::vector<std::vector<bool>> nextGrid;

// Camera and zoom variables
extern float zoom;
extern int offsetX, offsetY;
extern bool dragginge;
extern int lastMouseX, lastMouseY;

extern Button buttons[] ;



void handleEvents(SDL_Event &event);

void handleMouseMotion(SDL_Event &event);

void handleMouseWheel(SDL_Event &event);

void render(SDL_Renderer *renderer, TTF_Font *font);

int countNeighbors(int x, int y);

void update();

void toggleCell(SDL_Event &event);

void randomize();


bool togglePause();

bool clearGrid();

bool randomizeGrid();

TTF_Font *loadFont();


#endif // FUNC_H
