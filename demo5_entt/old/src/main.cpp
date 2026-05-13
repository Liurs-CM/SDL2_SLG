#include "Game.h"
#include "GameLib.h"
#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "game initing...\n";
    if(TheGame::Instance()->init("SDL2 Tiled World", SCR_X, SCR_Y, SCR_W, SCR_H, false)) {
        while(TheGame::Instance()->running()) {
            TheGame::Instance()->handleEvents();
            TheGame::Instance()->update();
            TheGame::Instance()->render();
            TheGame::Instance()->syncFPS();
        }
    }
    else {
        std::cout << "game init failure - " << SDL_GetError();
        return -1;
    }
    std::cout << "game closing...\n";
    TheGame::Instance()->clean();
    return 0;
}
