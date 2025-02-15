#include <SDL2/SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>

// Window and grid settings
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int CELL_SIZE = 10;
const int GRID_WIDTH = WINDOW_WIDTH / CELL_SIZE;
const int GRID_HEIGHT = WINDOW_HEIGHT / CELL_SIZE;

// Game state variables
bool running = true;                                                                    // Whether the game is running
bool paused = true;                                                                     // Whether the simulation is paused
std::vector<std::vector<bool>> grid(GRID_WIDTH, std::vector<bool>(GRID_HEIGHT, false)); // Grid representing cell states

void randomize();
void toggleCell(SDL_Event &event);
// Function to handle user input
void handleEvents(SDL_Event &event)
{
    while (SDL_PollEvent(&event))
    { // Process all pending events
        if (event.type == SDL_QUIT)
        {
            running = false; // Exit the main loop when window is closed
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_SPACE)
            {
                paused = !paused; // Toggle pause state
            }
            else if (event.key.keysym.sym == SDLK_r)
            {
                // Randomize grid with live and dead cells
                randomize();
            }
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {

            toggleCell(event);
        }
    }
}

// Function to render the grid
void render(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set background color to black
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set cell color to white

    for (int x = 0; x < GRID_WIDTH; ++x)
    {
        for (int y = 0; y < GRID_HEIGHT; ++y)
        {
            if (grid[x][y])
            { // If cell is alive
                SDL_Rect cell = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
                SDL_RenderFillRect(renderer, &cell); // Draw filled rectangle
            }
        }
    }
    SDL_RenderPresent(renderer); // Present the rendered frame
    printf("Rendering frame...\n");
}

int main()
{
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event event;

    // Main loop
    while (running)
    {
        handleEvents(event); // Handle user input
        render(renderer);    // Draw the grid
        SDL_Delay(16);       // Control frame rate (~60 FPS)
    }

    // Clean up resources before exiting
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void toggleCell(SDL_Event &event)
{
    // Get grid coordinates from mouse click
    int x = event.button.x / CELL_SIZE;
    int y = event.button.y / CELL_SIZE;
    if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT)
    {
        grid[x][y] = !grid[x][y]; // Toggle cell state
    }
}

void randomize()
{
    srand(time(nullptr));
    for (int x = 0; x < GRID_WIDTH; ++x)
    {
        for (int y = 0; y < GRID_HEIGHT; ++y)
        {
            grid[x][y] = rand() % 2; // Assign 0 or 1 randomly
        }
    }
}
