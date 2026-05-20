#pragma once
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_render.h"
#include "GameStateMachine.h"

class Game
{
    public:
        bool init(const char* title, int x_pos, int y_pos, int width, int height, bool fullscreen);
        void render();
        void update();
        void handleEvents();
        void clean(); 
        //function to access the private running variable
	    void quit() { m_bRunning = false; }
        GameStateMachine* getStateMachine() { return m_pGameStateMachine; }
        void setPlayerLives(int lives) { m_playerLives = lives; }
        int getPlayerLives() { return m_playerLives; }
        bool changingState() { return m_bChangingState; }
        void changingState(bool cs) { m_bChangingState = cs; }
        void setCurrentLevel(int currentLevel);
        const int getCurrentLevel() { return m_currentLevel; }
        void setNextLevel(int nextLevel) { m_nextLevel = nextLevel; }
        const int getNextLevel() { return m_nextLevel; }
        void setLevelComplete(bool levelComplete) { m_bLevelComplete = levelComplete; }
        const bool getLevelComplete() { return m_bLevelComplete; }
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
        float getScrollSpeed() { return m_scrollSpeed; }
        std::vector<std::string> getLevelFiles() { return m_levelFiles; }

    private:
        Game();
        ~Game();
        int m_gameWidth;
        int m_gameHeight;
        Uint32 frameStart;
        Uint32 frameTime;
        SDL_Window* m_pWindow;
        SDL_Renderer* m_pRenderer;
        bool m_bRunning;
        GameStateMachine* m_pGameStateMachine;
        int m_playerLives;
        float m_scrollSpeed;
        bool m_bLevelComplete;
        bool m_bChangingState;
        std::vector<std::string> m_levelFiles;
        int m_currentLevel;
        int m_nextLevel;
        static Game* s_pInstance;
};

typedef Game TheGame;


