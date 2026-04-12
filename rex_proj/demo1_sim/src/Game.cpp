#include "Game.h"
#include "RenderContext.h"
#include "SDL_image.h"
#include <SDL2_gfxPrimitives.h>
#include <iostream>

Game* Game::s_pInstance = 0;

bool Game::init(const char* title, int x_pos, int y_pos, int width,
        int height, int flags)
{
    //initialize SDL
    if( SDL_Init(SDL_INIT_EVERYTHING) >= 0 )
    {
        std::cout << "SDL init success\n";
        //init the window
        m_pWindow = SDL_CreateWindow(title, x_pos, y_pos,
                width, height, flags);
        if( m_pWindow != 0)
        {
            std::cout << "window creation success\n";
            m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
            if( m_pRenderer != 0)
            {
                RenderContext::set(m_pRenderer);
                std::cout << "renderer creation success\n";
                //SDL_SetRenderDrawColor(m_pRenderer, 128,128,128,255);
                SDL_SetRenderDrawColor(m_pRenderer, 255,0,255,255);
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
        std::cout << "SDL init fail\n";
        return false;
    }

    std::cout << "game initing success!\n";
    m_bRunning = true;

    if(!TheTextureManager::Instance()->load("assets/animate.png", "animate"))
    {
        std::cout << "load img to texture fail\n";
        return false;
    }

    return true;
}

void Game::render()
{
    //clear the renderer to the draw color on the screen
    SDL_RenderClear(m_pRenderer);
    //move texture to render
    TheTextureManager::Instance()->drawFrame("animate", 100,100, 128, 82, 1, m_currentFrame);
    TheShape::Instance()->Shape_genrate();
    SDL_SetRenderDrawColor(m_pRenderer, 255,0,255,255);
    //draw to the screen
    SDL_RenderPresent(m_pRenderer);
}

void Game::update()
{
   m_currentFrame = int(((SDL_GetTicks() / 100) % 6));
} 

void Game::handleEvents()
{
    SDL_Event event;
    if( SDL_PollEvent(&event) )
    {
        switch (event.type)
        {
            case SDL_QUIT:
                m_bRunning = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    m_bRunning = false;
                    break;
                }
            default:
                break;
        }
    }
}

void Game::clean()
{
    std::cout << "cleaning game\n";
    SDL_DestroyWindow(m_pWindow);
    SDL_DestroyRenderer(m_pRenderer);
    SDL_Quit();
}

void Game::syncFPS()
{
    frameTime = SDL_GetTicks() - frameStart;
    if(frameTime < DELAY_TIME)
    {
        SDL_Delay((int)(DELAY_TIME - frameTime));
    }
    frameStart = SDL_GetTicks();
}
