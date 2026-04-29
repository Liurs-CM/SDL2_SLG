#include "Game.h"
#include "Shape.h"
#include "RenderContext.h"
#include "InputHandler.h"
#include "GameObjectFactory.h"
#include "PlayState.h"
#include "MenuMainState.h"
#include "MenuOverState.h"
#include "MenuButton.h"
#include "AnimatedGraphic.h"
#include "ScrollingBackground.h"
#include "SoundManager.h"
#include "Cat.h"
#include "Level1Boss.h"
//#include "ShotGlider.h"
//#include "RoofTurret.h"
//#include "Eskeletor.h"
#include <iostream>

Game* Game::s_pInstance = 0;

Game::Game():
    m_pWindow(0),
    m_pRenderer(0),
    m_bRunning(false),
    m_pGameStateMachine(0),
    m_scrollSpeed(1),
    m_bLevelComplete(false),
    m_bChangingState(false)
{
    // add some level files to an array
    m_levelFiles.push_back("assets/map1.tmx");
    //m_levelFiles.push_back("assets/map2.tmx");
    // start at this level
    m_currentLevel = 1;
}

Game::~Game()
{
    // we must clean up after ourselves to prevent memory leaks
    m_pRenderer= 0;
    m_pWindow = 0;
}

bool Game::init(const char* title, int x_pos, int y_pos, int width, int height, bool fullscreen)
{
    //initialize SDL
    if( SDL_Init(SDL_INIT_EVERYTHING) >= 0 )
    {
        int flags = 0;
        if(fullscreen) { flags = SDL_WINDOW_FULLSCREEN; }
        std::cout << "SDL init success\n";
        //init the window
        m_pWindow = SDL_CreateWindow(title, x_pos, y_pos, width, height, flags);
        if( m_pWindow != 0)
        {
            std::cout << "window creation success\n";
            m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);
            if( m_pRenderer != 0)
            {
                RenderContext::set(m_pRenderer);
                std::cout << "renderer creation success\n";
                SDL_SetRenderDrawColor(m_pRenderer, 128,128,128,255);
                //SDL_SetRenderDrawColor(m_pRenderer, 255,0,255,255);
            }
            else
            {
                std::cout << "renderer init fail\n";
                return false;
            }
        }
        else
        {
            std::cout << "window init fail\n";
            return false;
        }
    }
    else
    {
        std::cout << "SDL init fail!\nSDL ERROR: " << SDL_GetError();
        return false;
    }

    TheInputHandler::Instance()->initialiseJoysticks();
    std::cout << "game initing success!\n";
    TheSoundManager::Instance()->load("assets/DST_ElecRock.ogg", "music1", SOUND_MUSIC);
    TheSoundManager::Instance()->load("assets/boom.wav", "explode", SOUND_SFX);
    TheSoundManager::Instance()->load("assets/phaser.wav", "shoot", SOUND_SFX);
    TheSoundManager::Instance()->playMusic("music1", -1);
    TheGameObjectFactory::Instance()->registerType("MenuButton", new MenuButtonCreator());
    TheGameObjectFactory::Instance()->registerType("ScrollingBackground", new ScrollingBackgroundCreator());
	TheGameObjectFactory::Instance()->registerType("Player", new PlayerCreator());
	TheGameObjectFactory::Instance()->registerType("Cat", new CatCreator());
    TheGameObjectFactory::Instance()->registerType("Level1Boss", new Level1BossCreator());
    //TheGameObjectFactory::Instance()->registerType("AnimatedGraphic", new AnimatedGraphicCreator());
    //TheGameObjectFactory::Instance()->registerType("Turret", new TurretCreator());
    //TheGameObjectFactory::Instance()->registerType("Glider", new GliderCreator());
    //TheGameObjectFactory::Instance()->registerType("ShotGlider", new ShotGliderCreator());
    //TheGameObjectFactory::Instance()->registerType("RoofTurret", new RoofTurretCreator());
    //TheGameObjectFactory::Instance()->registerType("Eskeletor", new EskeletorCreator());
	m_pGameStateMachine = new GameStateMachine();
    m_pGameStateMachine->changeState(new MenuMainState());
    m_bRunning = true;
    return true;
}

void Game::setCurrentLevel(int currentLevel)
{
    m_currentLevel = currentLevel;
    m_pGameStateMachine->changeState(new MenuOverState());
    m_bLevelComplete = false;
}


void Game::render()
{
    //clear the renderer to the draw color on the screen
    SDL_RenderClear(m_pRenderer);
    //move texture to render
    TheShape::Instance()->Shape_genrate();
	m_pGameStateMachine->render();
    SDL_SetRenderDrawColor(m_pRenderer, 128,128,128,255);
    //draw to the screen
    SDL_RenderPresent(m_pRenderer);
}

void Game::update()
{
    m_pGameStateMachine->update();
} 

void Game::handleEvents()
{
    TheInputHandler::Instance()->update();
	if(TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RETURN))
	{
		m_pGameStateMachine->changeState(new PlayState());
	}
}

void Game::clean()
{
    TheInputHandler::Instance()->clean();
    m_pGameStateMachine->clean();
    m_pGameStateMachine = 0;
    delete m_pGameStateMachine;
    TheTextureManager::Instance()->clearTextureMap();
    SDL_DestroyWindow(m_pWindow);
    SDL_DestroyRenderer(m_pRenderer);
    std::cout << "cleaning game\n";
    SDL_Quit();
}

void Game::syncFPS()
{
    AnimatedGraphic::updateGlobalFrame();
    frameTime = SDL_GetTicks() - frameStart;
    if(frameTime < DELAY_TIME_MS)
    {
        SDL_Delay((int)(DELAY_TIME_MS - frameTime));
    }
    frameStart = SDL_GetTicks();
    //std::cout << DELAY_TIME_MS << " " << frameTime << "\n";
}
