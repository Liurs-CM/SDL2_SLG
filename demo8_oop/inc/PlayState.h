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
        uint16_t m_turnNum = 0;
        struct InputData {
             uint8_t selectedUnit = -1;
             bool commandConfirmed = false;
        } m_inputData;
        struct AIData {
             uint8_t selectedUnit = -1;
             bool commandConfirmed = false;
        } m_AIData;
        struct ExecutionData {
             uint16_t m_animTimer = 0;
             bool finished = false;
        } m_execData;
        void handleInputPhase();
        void handleExecutionPhase();
        void enterInputPhase();
        void enterExecutionPhase();
        void resetInputData();
        void resetExecData();
};

