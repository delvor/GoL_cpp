#ifndef UI_BUTTON_H_
#define UI_BUTTON_H_

#include <SDL2/SDL_rect.h>
#include <string>

// Button structure

struct Button
{

    SDL_Rect rect;
    std::string label;
    bool (*action)(); // Function pointer for action when clicked
};

#endif // UI_BUTTON_H_