//
//  PauseState.cpp
//  SDL Game Programming Book
//
//  Created by shaun mitchell on 16/02/2013.
//  Copyright (c) 2013 shaun mitchell. All rights reserved.
//

#include "PauseState.h"
#include "MainMenuState.h"
#include "PlayState.h"
#include "TextureManager.h"
#include "Game.h"
#include "MenuButton.h"
#include "InputHandler.h"
#include "StateParser.h"

const std::string PauseState::s_pauseID = "PAUSE";

void PauseState::s_pauseToMain()
{
    TheGame::Instance()->getStateMachine()->changeState(new MainMenuState());
}

void PauseState::s_resumePlay()
{
    TheGame::Instance()->getStateMachine()->popState();
}

void PauseState::update()
{
    if(m_loadingComplete && !m_gameObjects.empty())
    {
        for(auto* obj : m_gameObjects)
        {
            obj->update();
        }
    }
}

void PauseState::render()
{
    if(m_loadingComplete && !m_gameObjects.empty())
    {
        for(auto* obj : m_gameObjects)
        {
            obj->draw();
        }
    }
}

bool PauseState::onEnter()
{
    StateParser stateParser;
    stateParser.parseState("assets/attack.xml", s_pauseID, &m_gameObjects, &m_textureIDList);
    
    m_callbacks.push_back(0);
    m_callbacks.push_back(s_pauseToMain);
    m_callbacks.push_back(s_resumePlay);
    
    setCallbacks(m_callbacks);
    
    m_loadingComplete = true;
    
    std::cout << "entering PauseState\n";
    return true;
}

bool PauseState::onExit()
{
    if(m_loadingComplete && !m_gameObjects.empty())
    {
        for(auto* obj : m_gameObjects)
        {
            obj->clean();
            delete obj;
        }
        m_gameObjects.clear();
    }
    // clear the texture manager
    for(const auto& obj : m_textureIDList)
    {
        TheTextureManager::Instance()->clearFromTextureMap(obj);
    }
    TheInputHandler::Instance()->reset();
    
    std::cout << "exiting PauseState\n";
    return true;
}

void PauseState::setCallbacks(const std::vector<Callback>& callbacks)
{
    // go through the game objects
    if(!m_gameObjects.empty())
    {
        for(auto* obj : m_gameObjects)
        {
            // if they are of type MenuButton then assign a callback based on the id passed in from the file
            if(dynamic_cast<MenuButton*>(obj))
            {
                MenuButton* pButton = dynamic_cast<MenuButton*>(obj);
                pButton->setCallback(callbacks[pButton->getCallbackID()]);
            }
        }
    }
}

