//
//  Game.h
//  SDL Game Programming Book
//
//

#ifndef __SDL_Game_Programming_Book__Game__
#define __SDL_Game_Programming_Book__Game__

#include "SDL.h"
#include "GameStateMachine.h"
//#include "SDL_mixer.h"
#include <vector>

class Game
{
public:
    static Game* Instance()
    {
        if(s_pInstance == 0)
        {
            s_pInstance = new Game();
            return s_pInstance;
        }
        return s_pInstance;
    }
    bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void render();
    void update();
    void handleEvents();
    void clean();
    SDL_Renderer* getRenderer() const { return m_pRenderer; }
    SDL_Window* getWindow() const { return m_pWindow; }
    GameStateMachine* getStateMachine() { return m_pGameStateMachine; }
    void setPlayerLives(int lives) { m_playerLives = lives; }
    int getPlayerLives() { return m_playerLives; }
    void setCurrentLevel(int currentLevel);
    const int getCurrentLevel() { return m_currentLevel; }
    void setNextLevel(int nextLevel) { m_nextLevel = nextLevel; }
    const int getNextLevel() { return m_nextLevel; }
    void setLevelComplete(bool levelComplete) { m_bLevelComplete = levelComplete; }
    const bool getLevelComplete() { return m_bLevelComplete; }
    bool running() { return m_bRunning; }
    void quit() { m_bRunning = false; }
    int getGameWidth() const { return m_gameWidth; }
    int getGameHeight() const { return m_gameHeight; }
    float getScrollSpeed() { return m_scrollSpeed; }
	bool changingState() { return m_bChangingState; }
	void changingState(bool cs) { m_bChangingState = cs; }
    std::vector<std::string> getLevelFiles() { return m_levelFiles; }
private:
    Game();
    ~Game();
    Game(const Game&);
	Game& operator=(const Game&);
    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;
    bool m_bRunning;
    GameStateMachine* m_pGameStateMachine;
    int m_playerLives;
    float m_scrollSpeed;
    bool m_bLevelComplete;
    bool m_bChangingState;
    static Game* s_pInstance;
    int m_gameWidth;
    int m_gameHeight;
    int m_currentLevel;
    int m_nextLevel;
    std::vector<std::string> m_levelFiles;
};

typedef Game TheGame;

#endif /* defined(__SDL_Game_Programming_Book__Game__) */
