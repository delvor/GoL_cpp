#ifndef GOL_H
#define GOL_H

#include "../ui/Button.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

class GameOfLife
{
    // Window and grid settings
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    const int CELL_SIZE = 1;
    int GRID_WIDTH;
    int GRID_HEIGHT;

    // Game state variables
    bool running;
    bool paused;
    std::vector<std::vector<bool>> grid;
    std::vector<std::vector<bool>> nextGrid;

    // Camera and zoom variables
    float zoom;
    int offsetX, offsetY;
    bool dragging;
    int lastMouseX, lastMouseY;

    Button buttons[3];

public:
    GameOfLife();

    int run();
    bool togglePause();
    bool clearGrid();
    bool randomizeGrid();

private:
    void handleEvents(SDL_Event &event);
    void handleMouseMotion(SDL_Event &event);
    void handleMouseWheel(SDL_Event &event);
    void render(SDL_Renderer *renderer, TTF_Font *font);
    int countNeighbors(int x, int y);
    void update();
    void toggleCell(SDL_Event &event);
    void randomize();
    TTF_Font *loadFont();
};

#endif // GOL_H
