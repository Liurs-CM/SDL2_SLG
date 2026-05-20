#pragma once
#include "MenuState.h"
#include "GameObject.h"
#include <memory>
#include <vector>

class MenuPauseState : public MenuState
{
    public:
        virtual ~MenuPauseState() {}
        virtual void update();
        virtual void render();
        virtual bool onEnter();
        virtual bool onExit();
        virtual std::string getStateID() const { return s_pauseID; }
        virtual void setCallbacks(const std::vector<Callback>& callbacks);
    private:
        static const std::string s_pauseID;
        static void s_pauseToMain();
        static void s_resumePlay();
        std::vector<std::unique_ptr<GameObject>> m_gameObjects;
};

