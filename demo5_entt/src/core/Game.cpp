#include "Game.hpp"
#include "constants.hpp"
#include "sys/house.hpp"
#include "sys/render.hpp"
#include "sys/eat_dots.hpp"
#include "sys/movement.hpp"
#include "core/factories.hpp"
#include "sys/set_target.hpp"
#include "sys/player_input.hpp"
#include "sys/pursue_target.hpp"
#include "sys/change_ghost_mode.hpp"
#include "sys/player_ghost_collide.hpp"

void Game::init() {
    maze = makeMazeState();
    const entt::entity player = makePlayer(reg);
    const entt::entity blinky = makeBlinky(reg, player);
    makePinky(reg, player);
    makeInky(reg, player, blinky);
    makeClyde(reg, player);
    // seeding a pseudo random number generator with a random source
    rand.seed(std::random_device{}());
}

void Game::input(const SDL_Scancode key) {
    if (state == State::playing) {
        playerInput(reg, key);
    }
}

bool Game::logic() {
    if (state != State::playing) {
        return true;
    }

    if (scattering) {
        if (ticks >= scatterTicks) {
            ghostChase(reg);
            ticks = 0;
            scattering = false;
        }
    } else {
        if (ticks >= chaseTicks) {
            ghostScatter(reg);
            ticks = 0;
            scattering = true;
        }
    }
    ++ticks;

    movement(reg);
    wallCollide(reg, maze);
    dots += eatDots(reg, maze);
    if (eatEnergizer(reg, maze)) {
        ghostScared(reg);
    }
    ghostScaredTimeout(reg);
    enterHouse(reg);
    setBlinkyChaseTarget(reg);
    setPinkyChaseTarget(reg);
    setInkyChaseTarget(reg);
    setClydeChaseTarget(reg);
    setScaredTarget(reg, maze, rand);
    setScatterTarget(reg);
    setEatenTarget(reg);
    leaveHouse(reg);
    pursueTarget(reg, maze);

    const GhostCollision collision = playerGhostCollide(reg);
    if (collision.type == GhostCollision::Type::eat) {
        ghostEaten(reg, collision.ghost);
    }
    if (collision.type == GhostCollision::Type::lose) {
        state = State::lost;
    } else if (dots == dotsInMaze) {
        state = State::won;
    }
    return true;
}

void Game::render(SDL::QuadWriter &writer, const int frame) {
    if (state == State::playing) {
        fullRender(writer, animera::SpriteID::maze);
        dotRender(writer, maze);
        playerRender(reg, writer, frame);
        ghostRender(reg, writer, frame);
    } else if (state == State::won) {
        fullRender(writer, animera::SpriteID::win);
    } else if (state == State::lost) {
        fullRender(writer, animera::SpriteID::lose);
    }
}
