#ifndef __Game__
#define __Game__
#include "SDL.h"
#include "GameLib.h"
#include "Vector2D.h"
#include "TextureManager.h"
#include "Shape.h"
#include "GameStateMachine.h"
#include "PlayState.h"

class Game
{
    public:
        ~Game() {}
        bool init(const char* title, int x_pos, int y_pos, int width, int height, bool fullscreen);
        void render();
        void update();
        void handleEvents();
        void clean(); 
	    void quit() { m_bRunning = false; }
        GameStateMachine* getStateMachine() { return m_pGameStateMachine; }
        //function to access the private running variable
        bool running() { frameStart = SDL_GetTicks(); return m_bRunning; }
        void syncFPS();
        Uint32 getframeTime() { return frameTime; }
        static Game* Instance()
        {
            if(s_pInstance == 0)
            {
                s_pInstance = new Game();
                return s_pInstance;
            }
            return s_pInstance;
        }
        int getGameWidth() const { return m_gameWidth; }
        int getGameHeight() const { return m_gameHeight; }

    private:
        Game() {}
        int m_gameWidth;
        int m_gameHeight;
        Uint32 frameStart;
        Uint32 frameTime;
        bool m_bRunning;
        SDL_Window* m_pWindow;
        SDL_Renderer* m_pRenderer;
        GameStateMachine* m_pGameStateMachine;
        static Game* s_pInstance;
};

typedef Game TheGame;

#endif /* defined(__Game__) */
