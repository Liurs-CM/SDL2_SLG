#include "PlayState.h"
#include "InputHandler.h"
#include "MenuPauseState.h"
#include "MenuOverState.h"
#include "LevelParser.h"
#include "Level.h"
#include "Game.h"
#include <string>
#include <algorithm>
#include <iostream>

const std::string PlayState::s_playID = "PLAY";

void PlayState::update()
{
    if(m_loadingComplete && !m_exiting)
    {
        if(TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
        {
            TheGame::Instance()->getStateMachine()->pushState(new MenuPauseState());
        }
        if(TheGame::Instance()->getPlayerLives() == 0)
        {
            TheGame::Instance()->getStateMachine()->changeState(new MenuOverState());
        }
        if(pLevel != 0)
        {
            pLevel->update();
        }
    }
}

void PlayState::render()
{
    if(m_loadingComplete)
    {
        if(pLevel != 0)
        {
            pLevel->render();
        }
    }
}

bool PlayState::onEnter()
{
    TheGame::Instance()->setPlayerLives(3);
    LevelParser levelParser;
    pLevel = levelParser.parseLevel(TheGame::Instance()->getLevelFiles()[TheGame::Instance()->getCurrentLevel() - 1].c_str());
    if(pLevel != 0) {
        m_loadingComplete = true;
    } else {
        std::cout << "Level " << TheGame::Instance()->getCurrentLevel() << " Load Failed" << std::endl;
    }
    std::cout << "entering PlayState" << std::endl;
    return true;
}

bool PlayState::onExit()
{
    m_exiting = true;
    TheInputHandler::Instance()->reset_mouseButtonState();
    std::cout << "exiting PlayState" << std::endl;
    return true;
}

