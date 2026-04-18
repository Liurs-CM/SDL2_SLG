#include "MenuPauseState.h"
#include "MenuButton.h"
#include "MenuMainState.h"
#include "StateParser.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "PlayState.h"
#include "Game.h"
#include <iostream>

const std::string MenuPauseState::s_pauseID = "PAUSE";

void MenuPauseState::s_pauseToMain()
{
    TheGame::Instance()->getStateMachine()->changeState(new MenuMainState());
}

void MenuPauseState::s_resumePlay()
{
    TheGame::Instance()->getStateMachine()->popState();
}

void MenuPauseState::update()
{
    for(auto& obj : m_gameObjects) {
        obj->update();
    }
}

void MenuPauseState::render()
{
    for(auto& obj : m_gameObjects) {
        obj->draw();
    }
}

bool MenuPauseState::onEnter()
{
    StateParser stateParser;
    stateParser.parseState("assets/test.xml", s_pauseID, &m_gameObjects, &m_textureIDList);
    m_callbacks.push_back(0);
    m_callbacks.push_back(s_pauseToMain);
    m_callbacks.push_back(s_resumePlay);
    setCallbacks(m_callbacks);
    std::cout << "entering MenuPauseState\n";
    return true;
}

bool MenuPauseState::onExit()
{
    for(auto& obj : m_gameObjects) {
        obj->clean();
    }
    m_gameObjects.clear();
    // clear the texture manager
    for(const auto& obj : m_textureIDList)
    {
        TheTextureManager::Instance()->clearFromTextureMap(obj);
    }
    std::cout << "exiting MenuPauseState\n";
    return true;
}

void MenuPauseState::setCallbacks(const std::vector<Callback>& callbacks)
{
    for(auto& obj : m_gameObjects)
    {
        if(dynamic_cast<MenuButton*>(obj.get()) != nullptr)
        {
            MenuButton* pButton = dynamic_cast<MenuButton*>(obj.get());
            pButton->setCallback(callbacks[pButton->getCallbackID()]);
        }
    }
}

