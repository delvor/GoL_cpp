#include "game/gol.h"

int main(int argc, char *args[])

{

    GameOfLife *game = new GameOfLife();
    int result = game->run();
    delete game;
    return result;
}
