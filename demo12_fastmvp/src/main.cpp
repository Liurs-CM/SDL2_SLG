#include <iostream>
#include "core/Game.h"
#include "core/GameConfig.h"

int main(int, char**) {
    if(TheGame::Instance()->init("SDL2 EnTT World", {SCR_X, SCR_Y}, {SCR_W, SCR_H}, false))
    {
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
