module;
//#include <entt/entt.hpp>
//#include <SDL.h>
export module system.input;
import entt;

export void inputSystem(entt::registry& reg, bool& running) {
    //SDL_Event e;
    //while (SDL_PollEvent(&e)) {
    //    if (e.type == SDL_QUIT) running = false;
    //}

    //const Uint8* keystate = SDL_GetKeyboardState(nullptr);
    //bool enterPressed = keystate[SDL_SCANCODE_RETURN];

    // 只遍历带 PlayerTag 的实体
    //auto view = reg.view<PlayerTag, Velocity>();
    //for (auto entity : view) {
    //    auto& vel = view.get<Velocity>(entity);
    //    vel.dx = 0; vel.dy = 0;
    //    if (keystate[SDL_SCANCODE_LEFT])  vel.dx = -1;
    //    if (keystate[SDL_SCANCODE_RIGHT]) vel.dx =  1;
    //    if (keystate[SDL_SCANCODE_UP])    vel.dy = -1;
    //    if (keystate[SDL_SCANCODE_DOWN])  vel.dy =  1;
    //}

    // 状态切换：回车键触发从主菜单进入游戏（原本的 handleEvents 逻辑）
    //if (enterPressed) {
    //    auto stateView = reg.view<GameState>();
    //    if (!stateView.empty()) {
    //        auto& state = stateView.get<GameState>(*stateView.begin());
    //        if (state.current == GameStateType::MainMenu) {
    //            state.current = GameStateType::Playing;
    //            state.changing = true;
    //        }
    //    }
    //}
}
