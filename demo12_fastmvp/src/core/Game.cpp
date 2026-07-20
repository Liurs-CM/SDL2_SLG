#include "core/Game.h"
#include "core/InputHandler.h"
#include "core/GameConfig.h"
#include "core/Factories.hpp"

Game* Game::s_pInstance = 0;

Game::Game():
    m_pWindow(0),
    m_pRenderer(0),
    m_bRunning(false)
{}

Game::~Game()
{
    // we must clean up after ourselves to prevent memory leaks
    m_pRenderer= 0;
    m_pWindow = 0;
}

bool Game::init(const char* title, vec pos, vec size, bool fullscreen)
{
    int flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "SDL init fail! " << SDL_GetError() << "\n";
        return false;
    }
    std::cout << "SDL init success\n";

    m_pWindow = SDL_CreateWindow(title, pos.x, pos.y, size.x, size.y, flags);
    if(!m_pWindow) {
        std::cout << "window init fail\n";
        SDL_Quit();
        return false;
    }
    std::cout << "window creation success\n";

    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);
    if(!m_pRenderer) {
        std::cout << "renderer init fail\n";
        return false;
    }
    m_renderContext.set(m_pRenderer);
    std::cout << "renderer creation success\n";
    //SDL_SetRenderDrawColor(m_pRenderer, 128,128,128,255);

    TheInputHandler::Instance()->initialiseJoysticks();
    std::cout << "game initing...\n";
    m_renderSystem.setRenderer(m_pRenderer, &registry);
    //m_mapParserSystem.parseLevel(registry,"assets/map2.tmx");
    m_mapParserSystem.parseLevel(registry,"assets/map3.tmx");
    m_mapGenSystem.generate(registry, 1234567890);
    m_mapParserSystem.precomputeTileUVs(registry);
    makeObj(registry, {9, 8}, "blocks_item", objID::Cat, 2, 1, 1, 2);
    makeObj(registry, {8, 8}, "blocks_item", objID::Coin, 2, 1, 1, 2);
    makeObj(registry, {7, 8}, "blocks_item", objID::Chest+1, 2, 1, 1, 2);
    makeObj(registry, {6, 8}, "blocks_item", objID::Chest, 2, 1, 1, 2);
    //TheSoundManager::Instance()->load("assets/phaser.wav", "shoot", SOUND_SFX);
    //TheSoundManager::Instance()->playMusic("music1", -1);
	//m_pGameStateMachine = new GameStateMachine();
    //m_pGameStateMachine->changeState(new MenuMainState());
    m_bRunning = true;
    return true;
}

void Game::handleEvents()
{
    TheInputHandler::Instance()->update();
    m_inputSystem.update(registry);
	//if(TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RETURN))
	//{
	//	m_pGameStateMachine->changeState(new PlayState());
	//}
}

void Game::update()
{
    m_moveSystem.update(registry);
    m_cameraSystem.update(registry);
    m_destroySystem.flushDestroyedEntities(registry);
    //m_pGameStateMachine->update();
} 

void Game::render()
{
    SDL_SetRenderDrawColor(m_pRenderer, 128,128,128,255);
    SDL_RenderClear(m_pRenderer);
    m_renderSystem.update();
    SDL_RenderPresent(m_pRenderer);
}

void Game::syncFPS()
{
    //AnimatedGraphic::updateGlobalFrame();
    frameTime = SDL_GetTicks() - frameStart;
    if(frameTime < DELAY_TIME_MS)
    {
        SDL_Delay((int)(DELAY_TIME_MS - frameTime));
    }
    frameStart = SDL_GetTicks();
    //std::cout << DELAY_TIME_MS << " " << frameTime << "\n";
}

void Game::clean()
{
    TheInputHandler::Instance()->clean();
    SDL_DestroyWindow(m_pWindow);
    SDL_DestroyRenderer(m_pRenderer);
    std::cout << "cleaning game\n";
    SDL_Quit();
}

