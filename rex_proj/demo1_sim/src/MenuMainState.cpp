#include "MenuMainState.h"
#include "MenuButton.h"
#include "StateParser.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "PlayState.h"
#include "Game.h"
#include <iostream>
#include <assert.h>

const std::string MenuMainState::s_menuID = "MENU";

// Callbacks
void MenuMainState::s_menuToPlay()
{
    TheGame::Instance()->getStateMachine()->changeState(new PlayState());
}

void MenuMainState::s_exitFromMenu()
{
    TheGame::Instance()->quit();
}
// end callbacks

void MenuMainState::update()
{
    if(TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_SPACE)) {
        s_menuToPlay();
    }
    for(auto& obj : m_gameObjects) {
        obj->update();
    }
}

void MenuMainState::render()
{
    for(auto& obj : m_gameObjects) {
        obj->draw();
    }
}

bool MenuMainState::onEnter()
{
    // parse the state
    StateParser stateParser;
    stateParser.parseState("assets/test.xml", s_menuID, &m_gameObjects, &m_textureIDList);
    m_callbacks.push_back(0);
    m_callbacks.push_back(s_menuToPlay);
    m_callbacks.push_back(s_exitFromMenu);
    setCallbacks(m_callbacks);
    std::cout << "entering MenuMainState\n";
    return true;
}

bool MenuMainState::onExit()
{
    // clean the game objects
    for(auto& obj :  m_gameObjects)
    {
        obj->clean();
    }
    m_gameObjects.clear();
    for(const auto& obj : m_textureIDList)
    {
        TheTextureManager::Instance()->clearFromTextureMap(obj);
    }
    TheInputHandler::Instance()->reset();
    std::cout << "exiting MenuMainState\n";
    return true;
}

void MenuMainState::setCallbacks(const std::vector<Callback>& callbacks)
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

