#include <iostream>
#include <exception>
#include <SDL_main.h>
#include "core/App.hpp"

extern "C" int main(int, char *[]) {
    try {
        Application app;
        app.run();
    } catch (std::exception &e) {
        // The only exceptions we should get are from SDL
        std::cout << e.what() << '\n';
        return 1;
    }
    return 0;
}
