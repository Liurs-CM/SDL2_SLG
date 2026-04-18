#include "MenuOverState.h"
#include "MenuMainState.h"
#include "MenuButton.h"
#include "StateParser.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "PlayState.h"
#include "Game.h"
#include <iostream>

const std::string MenuOverState::s_gameOverID = "GAMEOVER";

void MenuOverState::s_gameOverToMain()
{
    TheGame::Instance()->getStateMachine()->changeState(new MenuMainState());
}

void MenuOverState::s_restartPlay()
{
    TheGame::Instance()->getStateMachine()->changeState(new PlayState());
}

void MenuOverState::update()
{
    for(auto& obj : m_gameObjects)
    {
        obj->update();
    }
}

void MenuOverState::render()
{
    for(auto& obj : m_gameObjects) {
        obj->draw();
    }
}

bool MenuOverState::onEnter()
{
    // parse the state
    StateParser stateParser;
    stateParser.parseState("assets/test.xml", s_gameOverID, &m_gameObjects, &m_textureIDList);
    m_callbacks.push_back(0);
    m_callbacks.push_back(s_gameOverToMain);
    m_callbacks.push_back(s_restartPlay);
    setCallbacks(m_callbacks);
    std::cout << "entering MenuOverState\n";
    return true;
}

bool MenuOverState::onExit()
{
    for(auto& obj : m_gameObjects) {
        obj->clean();
    }
    m_gameObjects.clear();
    for(const auto& obj : m_textureIDList)
    {
        TheTextureManager::Instance()->clearFromTextureMap(obj);
    }
    TheInputHandler::Instance()->reset();
    std::cout << "exiting MenuOverState\n";
    return true;
}

void MenuOverState::setCallbacks(const std::vector<Callback>& callbacks)
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

