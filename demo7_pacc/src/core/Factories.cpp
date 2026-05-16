#include "Factories.hpp"
#include "comp/Dir.hpp"
#include "Constants.hpp"
#include "comp/House.hpp"
#include "comp/Ghost.hpp"
#include "comp/Sprite.hpp"
#include "comp/Target.hpp"
#include "comp/Player.hpp"
#include "comp/Position.hpp"
#include "comp/GhostMode.hpp"
#include "comp/ChaseTarget.hpp"
#include "comp/HomePosition.hpp"
#include <entt/entity/registry.hpp>

entt::entity makePlayer(entt::registry &reg) {
  const entt::entity e = reg.create();
  reg.emplace<Player>(e);
  reg.emplace<DesiredDir>(e, playerSpawnDir);
  reg.emplace<ActualDir>(e, playerSpawnDir);
  reg.emplace<Position>(e, playerSpawnPos);
  reg.emplace<PlayerSprite>(e, animera::SpriteID::pacman_beg_);
  return e;
}

namespace {

entt::entity makeGhost(entt::registry &reg, const Pos home, const Pos scatter) {
  const entt::entity e = reg.create();
  reg.emplace<Position>(e, home);
  reg.emplace<HomePosition>(e, home, scatter);
  reg.emplace<LeaveHouse>(e);
  reg.emplace<ScatterMode>(e);
  reg.emplace<DesiredDir>(e);
  reg.emplace<ActualDir>(e);
  reg.emplace<Target>(e);
  reg.emplace<Ghost>(e);
  return e;
}

}

entt::entity makeClyde(entt::registry &reg, const entt::entity player) {
  const entt::entity e = makeGhost(reg, clydeHome, clydeScatter);
  reg.emplace<ClydeChaseTarget>(e, player);
  reg.emplace<GhostSprite>(e, animera::SpriteID::clyde_beg_);
  return e;
}
