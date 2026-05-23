#include "PlayState.h"
#include "InputHandler.h"
#include "MenuPauseState.h"
#include "MenuOverState.h"
#include "LevelParser.h"
#include "Level.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Game.h"
#include <string>
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
            switch (m_currentPhase) {
                case Phase::INPUT:  handleInputPhase(); break;
                case Phase::EXECUTION: handleExecutionPhase(); break;
            }
            // 阶段切换逻辑
            if (m_currentPhase == Phase::INPUT && m_inputData.commandConfirmed) {
                resetExecData();
                m_currentPhase = Phase::EXECUTION;
                m_execData.m_animTimer = FPS;
                std::cout << "EXECUTION\n";
            }
            if (m_currentPhase == Phase::EXECUTION && m_execData.finished) {
                resetInputData();
                m_currentPhase = Phase::INPUT;
                std::cout << "INPUT\n";
            }
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
    TextureManager::Instance()->initHealthBarTexture();
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

void PlayState::handleInputPhase()
{
    if (TheInputHandler::Instance()->getMouseButtonState(LEFT))
    {
        pLevel->setPlayer2Pos(*(TheInputHandler::Instance()->getMousePosition()));
        m_inputData.commandConfirmed = true;
    }
    for (const auto& [key, v] : key2Vec) 
    {
        if (TheInputHandler::Instance()->isKeyDown(key))
        {
            pLevel->setPlayer2Pos(v, false);
            m_inputData.commandConfirmed = true;
        }
    }
    if(TheInputHandler::Instance()->isKeyPressed(SDL_SCANCODE_SPACE))
    {
        TheSoundManager::Instance()->playSound("shoot", 0);
    }
}

void PlayState::handleExecutionPhase()
{
    pLevel->movePlayer();
    if(m_execData.m_animTimer-- == 0){
        m_execData.finished = true;
    }
}

void PlayState::resetInputData() {
    m_inputData = InputData{}; // 重置为默认值
}

void PlayState::resetExecData() {
    m_execData = ExecutionData{};
}
