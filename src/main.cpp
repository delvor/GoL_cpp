#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <ctime>
#include <string>
#include "ui/Button.h"
#include <iostream>

// Window and grid settings
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int CELL_SIZE = 1;
int GRID_WIDTH = WINDOW_WIDTH / CELL_SIZE;
int GRID_HEIGHT = WINDOW_HEIGHT / CELL_SIZE;

// Game state variables
bool running = true;
bool paused = true;
std::vector<std::vector<bool>> grid(GRID_WIDTH, std::vector<bool>(GRID_HEIGHT, false));
std::vector<std::vector<bool>> nextGrid(GRID_WIDTH, std::vector<bool>(GRID_HEIGHT, false));

// Camera and zoom variables
float zoom = 1.0f;
int offsetX = 0, offsetY = 0;
bool dragging = false;
int lastMouseX, lastMouseY;

void randomize();
void toggleCell(SDL_Event &event);
void update();
void handleMouseMotion(SDL_Event &event);
void handleMouseWheel(SDL_Event &event);
void handleEvents(SDL_Event &event);
void render(SDL_Renderer *renderer, TTF_Font *font);
int countNeighbors(int x, int y);
bool togglePause();
bool clearGrid();
bool randomizeGrid();
TTF_Font *loadFont();

// Buttons
Button buttons[] = {
    {{10, 10, 100, 30}, "Pause", togglePause},
    {{120, 10, 100, 30}, "Clear", clearGrid},
    {{230, 10, 100, 30}, "Randomize", randomizeGrid},
};

int main(int argc, char *args[])
{
    SDL_Init(SDL_INIT_VIDEO);

    TTF_Init(); // Initialize SDL_ttf
    SDL_Window *window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font *font = loadFont();
    if (!font)
    {

        std::cerr << "Error loading font: %s\n"
                  << TTF_GetError() << std::endl;
        // printf("Error loading font: %s\n", TTF_GetError());
        return -1;
    }
    SDL_Event event;

    Uint32 lastUpdate = SDL_GetTicks();
    const Uint32 updateInterval = 200;

    while (running)
    {
        handleEvents(event);
        Uint32 currentTime = SDL_GetTicks();
        if (!paused && currentTime - lastUpdate >= updateInterval)
        {
            update();
            lastUpdate = currentTime;
        }
        render(renderer, font);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit(); // Quit SDL_ttf
    return 0;
}

void handleEvents(SDL_Event &event)
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_SPACE)
            {
                paused = !paused;
            }
            else if (event.key.keysym.sym == SDLK_r)
            {
                randomize();
            }
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (event.button.button == SDL_BUTTON_MIDDLE)
            {
                dragging = true;
                lastMouseX = event.button.x;
                lastMouseY = event.button.y;
            }
            else
            {
                toggleCell(event);
                for (Button &button : buttons)
                {
                    if (event.button.x >= button.rect.x && event.button.x <= button.rect.x + button.rect.w &&
                        event.button.y >= button.rect.y && event.button.y <= button.rect.y + button.rect.h)
                    {
                        button.action(); // Execute button's action
                    }
                }
            }
        }
        else if (event.type == SDL_MOUSEBUTTONUP)
        {
            if (event.button.button == SDL_BUTTON_MIDDLE)
            {
                dragging = false;
            }
        }
        else if (event.type == SDL_MOUSEMOTION)
        {
            handleMouseMotion(event);
        }
        else if (event.type == SDL_MOUSEWHEEL)
        {
            handleMouseWheel(event);
        }
    }
}

void handleMouseMotion(SDL_Event &event)
{
    if (dragging)
    {
        offsetX += event.motion.x - lastMouseX;
        offsetY += event.motion.y - lastMouseY;
        lastMouseX = event.motion.x;
        lastMouseY = event.motion.y;
    }
}

void handleMouseWheel(SDL_Event &event)
{
    float oldZoom = zoom;
    if (event.wheel.y > 0)
    {
        zoom *= 1.1f;
    }
    else if (event.wheel.y < 0)
    {
        zoom /= 1.1f;
    }
    offsetX = (offsetX * zoom) / oldZoom;
    offsetY = (offsetY * zoom) / oldZoom;
}

void render(SDL_Renderer *renderer, TTF_Font *font)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int x = 0; x < GRID_WIDTH; ++x)
    {
        for (int y = 0; y < GRID_HEIGHT; ++y)
        {
            if (grid[x][y])
            {
                int screenX = (x * CELL_SIZE - offsetX) * zoom;
                int screenY = (y * CELL_SIZE - offsetY) * zoom;
                int size = CELL_SIZE * zoom;
                SDL_Rect cell = {screenX, screenY, size, size};
                SDL_RenderFillRect(renderer, &cell);
            }
        }
    }

    // Render buttons
    for (const Button &button : buttons)
    {
        SDL_SetRenderDrawColor(renderer, 200, 200, 255, 255); // Button color
        SDL_RenderFillRect(renderer, &button.rect);

        // Render button text (simplified, more complex rendering requires SDL_ttf)
        // For simplicity, we'll just draw a rectangle here and you can later add text rendering
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Text color

        // You would render the text here with SDL_ttf or similar
        SDL_Color textColor = {0, 0, 0, 255}; // Text color (black)
        SDL_Surface *textSurface = TTF_RenderText_Solid(font, button.label.c_str(), textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        int textWidth = textSurface->w;
        int textHeight = textSurface->h;

        SDL_Rect textRect = {button.rect.x + (button.rect.w - textWidth) / 2,
                             button.rect.y + (button.rect.h - textHeight) / 2,
                             textWidth, textHeight};

        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

        // Clean up
        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);

        // For now, we'll just leave the label as a placeholder
    }

    SDL_RenderPresent(renderer);
}

int countNeighbors(int x, int y)
{
    int count = 0;
    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            if (dx == 0 && dy == 0)
                continue;
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < GRID_WIDTH && ny >= 0 && ny < GRID_HEIGHT)
            {
                count += grid[nx][ny];
            }
        }
    }
    return count;
}

void update()
{
    for (int x = 0; x < GRID_WIDTH; ++x)
    {
        for (int y = 0; y < GRID_HEIGHT; ++y)
        {
            int neighbors = countNeighbors(x, y);
            nextGrid[x][y] = (grid[x][y] && (neighbors == 2 || neighbors == 3)) || (!grid[x][y] && neighbors == 3);
        }
    }
    grid.swap(nextGrid);
}

void toggleCell(SDL_Event &event)
{
    int x = (event.button.x / zoom + offsetX) / CELL_SIZE;
    int y = (event.button.y / zoom + offsetY) / CELL_SIZE;
    if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT)
    {
        grid[x][y] = !grid[x][y];
    }
}

void randomize()
{
    srand(time(nullptr));
    for (int x = 0; x < GRID_WIDTH; ++x)
    {
        for (int y = 0; y < GRID_HEIGHT; ++y)
        {
            grid[x][y] = rand() % 2;
        }
    }
}

bool togglePause()
{
    paused = !paused;
    return true;
}

bool clearGrid()
{
    for (int x = 0; x < GRID_WIDTH; ++x)
    {
        for (int y = 0; y < GRID_HEIGHT; ++y)
        {
            grid[x][y] = false;
        }
    }
    return true;
}

bool randomizeGrid()
{
    randomize();
    return true;
}
TTF_Font *loadFont()
{

    // Detect platform and choose the font path
    const char *fontPath;

    // Get the platform name using SDL_GetPlatform
    const char *platform = SDL_GetPlatform();

    if (strcmp(platform, "Windows") == 0)
    {
        fontPath = "C:\\Windows\\Fonts\\Arial.ttf"; // Windows font path
    }
    else if (strcmp(platform, "Mac OS X") == 0)
    {
        fontPath = "/Library/Fonts/Arial.ttf"; // macOS font path
    }
    else
    {
        fontPath = "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"; // Linux font path
    }

    // Load the font with a size of 24
    TTF_Font *font = TTF_OpenFont(fontPath, 24);
    if (!font)
    {
        std::cerr << "Error loading font: %s\n"
                  << TTF_GetError() << std::endl;
        // printf("Error loading font: ", TTF_GetError());
        return nullptr;
    }
    return font;
}
