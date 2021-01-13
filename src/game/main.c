#include "game.h"

//ENTRY POINT
int main()
{
    struct Game* game = InitGame();
    GameLoop(game);
    return 0;
}