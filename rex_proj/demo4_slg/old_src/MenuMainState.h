#ifndef __MenuMainState__
#define __MenuMainState__
#include "MenuState.h"
#include <memory>
#include <vector>

class GameObject;

class MenuMainState : public MenuState
{
    public:
        virtual ~MenuMainState() {}
        virtual void update();
        virtual void render();
        virtual bool onEnter(); 
        virtual bool onExit(); 
        virtual std::string getStateID() const { return s_menuID; }
    private:
        static const std::string s_menuID;
        virtual void setCallbacks(const std::vector<Callback>& callbacks);
        // call back functions for menu items
        static void s_menuToPlay();
        static void s_exitFromMenu();
        std::vector<std::unique_ptr<GameObject>> m_gameObjects;
};

#endif // __MenuMainState__
