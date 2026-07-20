module;
#include <SDL.h>
#include <iostream>
#include <entt/entt.hpp>
#include <chrono>
//#include "SoundManager.h"
export module core.game;
import core.render_context;
import core.input_handler;
import system.input;

export namespace core {
    class Game {
        public:
            Game() : m_window(nullptr), m_renderer(nullptr), m_running(false) 
        {
            m_globalEntity = m_registry.create();
            //m_registry.emplace<GameState>(m_globalEntity, GameStateType::MainMenu, false);
            //m_registry.emplace<FrameControl>(m_globalEntity, 0u, 16u); // 约60fps
        }
            bool init(const char* title, int x, int y, int w, int h, bool fullscreen) {
                if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
                    std::cerr << "SDL init failed: " << SDL_GetError() << '\n';
                    return false;
                }
                Uint32 flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
                m_window = SDL_CreateWindow(title, x, y, w, h, flags);
                if (!m_window) return false;
                m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
                if (!m_renderer) return false;

                // 原来的工厂注册、资源加载等全部改为初始化实体/组件
                // 例如创建主菜单背景和按钮实体
                //auto bg = m_registry.create();
                //m_registry.emplace<ScrollingBackgroundTag>(bg);
                //m_registry.emplace<Position>(bg, 0.0f, 0.0f);
                //m_registry.emplace<Sprite>(bg, /* 纹理 */ nullptr, SDL_Rect{0,0,w,h}, w, h);

                // 音乐播放请求
                //auto soundEntity = m_registry.create();
                //m_registry.emplace<SoundRequest>(soundEntity, "music1", -1, true);

                m_running = true;
                return true;
            }
            bool running() const { return m_running; }

            void handleEvents() {
                inputSystem(m_registry, m_running);
            }

            void update() {
                // 计算 delta time（简化，也可用 FrameControl 组件）
                static auto previous = std::chrono::steady_clock::now();
                auto current = std::chrono::steady_clock::now();
                float dt = std::chrono::duration<float>(current - previous).count();
                previous = current;

                //// 系统执行顺序决定了游戏逻辑流
                //stateChangeSystem(m_registry);
                //movementSystem(m_registry, dt);
                //// 碰撞系统、AI系统...
                //soundSystem(m_registry);
            }

            void render() {
                renderSystem(m_registry, m_renderer);
            }
            void syncFPS() {
                auto view = m_registry.view<FrameControl>();
                auto& fc = view.get<FrameControl>(*view.begin());
                Uint32 frameTime = SDL_GetTicks() - fc.frameStart;
                if (frameTime < fc.delayTime) {
                    SDL_Delay(fc.delayTime - frameTime);
                }
                fc.frameStart = SDL_GetTicks();
            }

            void clean() {
                m_registry.clear();  // 销毁所有实体
                if (m_renderer) SDL_DestroyRenderer(m_renderer);
                if (m_window) SDL_DestroyWindow(m_window);
                SDL_Quit();
                m_running = false;
            }
        private:
            ~Game() { clean(); }
            SDL_Window* m_window;
            SDL_Renderer* m_renderer;
            entt::registry m_registry;
            entt::entity m_globalEntity; // 持有全局状态、帧控件的实体
            bool m_running;
    };
}
