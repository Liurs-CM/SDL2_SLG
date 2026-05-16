#pragma once
#include "maze.hpp"
#include "util/sdl_quad_writer.hpp"
#include <entt/entity/registry.hpp>
#include <SDL_scancode.h>
#include <random>

class Game {
    public:
        void init();
        void input(SDL_Scancode);
        bool logic();
        void render(SDL::QuadWriter &, int);

    private:
        enum class State {
            playing,
            won,
            lost
        };
        entt::registry reg;
        MazeState maze;
        int dots = 0;
        std::mt19937 rand;
        State state = State::playing;
        int ticks = 0;
        bool scattering = true;
};

