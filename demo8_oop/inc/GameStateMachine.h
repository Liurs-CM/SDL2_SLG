#pragma once
#include "GameState.h"
#include <vector>

class GameStateMachine
{
    public:
        GameStateMachine() : newState(nullptr), popStateFlag(false), changeStateFlag(false) {}
        void pushState(GameState* pState);
        void changeState(GameState* pState);
        void popState();
        void update();
        void render();
        void clean();
        std::vector<GameState*>& getGameStates() { return m_gameStates; }
    private:
        std::vector<GameState*> m_gameStates;
        void popStatePrivate();
        void changeStatePrivate(GameState* pState);
        GameState* newState;
        bool popStateFlag;
        bool changeStateFlag;
};

