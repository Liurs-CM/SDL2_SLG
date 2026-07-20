#include <iostream>
import core.game;

int main(int, char**) {
    core::Game game;
    std::cout << "game initing...\n";
    if(game.init("SDL2 Tiled World", SCR_X, SCR_Y, SCR_W, SCR_H, false)) {
        while(game.running()) {
            game.handleEvents();
            game.update();
            game.render();
            game.syncFPS();
        }
    }
    else {
        std::cerr << "初始化失败\n";
        return -1;
    }
    std::cout << "game closing...\n";
    game.clean();
    return 0;
}
