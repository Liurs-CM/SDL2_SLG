#include <iostream>
import core.game;

int main(int, char**) {
    core::Game game;
    if (!game.init("SDL2 C++20 MVP", 800, 600)) {
        std::cerr << "初始化失败\n";
        return -1;
    }
    game.run();
    game.shutdown();
    return 0;
}
