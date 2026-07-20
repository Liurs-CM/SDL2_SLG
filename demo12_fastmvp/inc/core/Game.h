#pragma once
#include "core/RenderContext.hpp"
#include "sys/RenderSystem.hpp"
#include "sys/MapParserSystem.hpp"
#include "sys/MapGeneratorSystem.hpp"
#include "sys/InputSystem.hpp"
#include "sys/MovePlayerSystem.hpp"
#include "sys/CameraSystem.hpp"
#include "sys/DestroySystem.hpp"
#include "util/Vector2D.h"
#include "entt/entt.hpp"
#include <SDL.h>


class Game
{
    public:
        bool init(const char* title, vec pos, vec size, bool fullscreen);
        void render();
        void update();
        void handleEvents();
        void clean(); 
        //function to access the private running variable
        bool running() { frameStart = SDL_GetTicks(); return m_bRunning; }
        void syncFPS();
        void quit() { m_bRunning = false; }
        static Game* Instance()
        {
            if(s_pInstance == 0)
            {
                s_pInstance = new Game();
                return s_pInstance;
            }
            return s_pInstance;
        }
    private:
        Game();
        ~Game();
        SDL_Window*         m_pWindow;
        SDL_Renderer*       m_pRenderer;
        bool                m_bRunning;
        Uint32              frameStart;
        Uint32              frameTime;
        static Game*        s_pInstance;
        entt::registry      registry;
        RenderContext       m_renderContext;
        RenderSystem        m_renderSystem;
        MapParserSystem     m_mapParserSystem;
        MapGeneratorSystem  m_mapGenSystem;
        InputSystem         m_inputSystem;
        MovePlayerSystem    m_moveSystem;
        CameraSystem        m_cameraSystem;
        DestroySystem       m_destroySystem;
};

typedef Game TheGame;
