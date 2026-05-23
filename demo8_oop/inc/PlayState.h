#pragma once
#include "GameState.h"
#include "Level.h"
#include <vector>

class GameObject;
class SDLGameObject;
class Level;

class PlayState : public GameState
{
    public:
        virtual ~PlayState() { delete pLevel; }
        virtual void update();
        virtual void render();
        virtual bool onEnter();
        virtual bool onExit();
        virtual std::string getStateID() const { return s_playID; }
    private:
        static const std::string s_playID;
        std::vector<GameObject*> m_gameObjects;
        Level* pLevel;
        // PlayPhase
        enum class Phase { INPUT, EXECUTION } m_currentPhase = Phase::INPUT;
        struct InputData {
             uint8_t selectedUnit = -1;
             bool commandConfirmed = false;
        } m_inputData;
        struct ExecutionData {
             int8_t m_animTimer = 0;
             bool finished = false;
        } m_execData;
        void handleInputPhase();
        void handleExecutionPhase();
        void resetInputData();
        void resetExecData();
};

