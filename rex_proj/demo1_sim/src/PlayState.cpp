#include "PlayState.h"
#include "InputHandler.h"
#include "MenuPauseState.h"
#include "MenuOverState.h"
#include "LevelParser.h"
#include "Level.h"
#include "BulletHandler.h"
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
        TheBulletHandler::Instance()->updateBullets();
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
        for(int i = 0; i < TheGame::Instance()->getPlayerLives(); i++)
        {
            TheTextureManager::Instance()->drawFrame("lives", i * 30, 0, 32, 30, 0, 0, 0.0);
        }
        TheBulletHandler::Instance()->drawBullets();
    }
}

bool PlayState::onEnter()
{
    TheGame::Instance()->setPlayerLives(8);
    LevelParser levelParser;
    pLevel = levelParser.parseLevel(TheGame::Instance()->getLevelFiles()[TheGame::Instance()->getCurrentLevel() - 1].c_str());
    TheTextureManager::Instance()->load("assets/bullet_B.png", "bullet1");
    TheTextureManager::Instance()->load("assets/bullet.png", "bullet2");
    TheTextureManager::Instance()->load("assets/bullet.png", "bullet3");
    TheTextureManager::Instance()->load("assets/lives.png", "lives");
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
    TheBulletHandler::Instance()->clearBullets();
    TheInputHandler::Instance()->reset_mouseButtonState();
    std::cout << "exiting PlayState" << std::endl;
    return true;
}

