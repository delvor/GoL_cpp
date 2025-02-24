#include <SDL2/SDL_ttf.h>
#include <ctime>
#include "main_func.h"
#include <iostream>


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
