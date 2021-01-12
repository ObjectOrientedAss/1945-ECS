#define SDL_MAIN_HANDLED //do not use SDL main.

#include "game.h"

//ENTRY POINT
int main()
{
    struct Game* game = StartGame();
    GameLoop(game);
    return 0;
}