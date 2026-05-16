#pragma once
#include "Maze.hpp"
#include "comp/Dir.hpp"
#include "util/SDL_QuadWriter.hpp"
#include <entt/entity/registry.hpp>
#include <SDL_scancode.h>
#include <random>

class Game {
    Dir pending_dir_;
    public:
        void init();
        void input(Dir dir);
        bool logic();
        void render(SDL::QuadWriter &, int);
        void set_pending_dir(Dir dir) { pending_dir_ = dir; }
        void clear_pending_dir() { pending_dir_ = Dir::none; }

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
