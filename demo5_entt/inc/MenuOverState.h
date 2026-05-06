#ifndef __MenuOverState__
#define __MenuOverState__
#include "MenuState.h"
#include "GameObject.h"
#include <memory>
#include <vector>

class MenuOverState : public MenuState
{
    public:
        virtual ~MenuOverState() {}
        virtual void update();
        virtual void render();
        virtual bool onEnter();
        virtual bool onExit();
        virtual std::string getStateID() const { return s_gameOverID; }
        virtual void setCallbacks(const std::vector<Callback>& callbacks);
    private:
        static const std::string s_gameOverID;
        static void s_gameOverToMain();
        static void s_restartPlay();
        std::vector<std::unique_ptr<GameObject>> m_gameObjects;
};

#endif // __MenuOverState__
