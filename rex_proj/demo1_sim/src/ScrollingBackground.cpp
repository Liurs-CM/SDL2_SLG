#include "ScrollingBackground.h"
#include "TextureManager.h"
#include "RenderContext.h"
#include "Game.h"

ScrollingBackground::ScrollingBackground() : SDLGameObject()
{
    count = 0;
    maxcount = 10;
}

void ScrollingBackground::load(std::unique_ptr<LoaderParams> const &pParams)
{
    SDLGameObject::load(std::move(pParams));
    m_scrollSpeed = pParams->getAnimSpeed();
    m_scrollSpeed = 1;
    m_srcRect1 = {0, 0, m_width, m_height};
    m_dstRect1 = {int(m_position.getX()), int(m_position.getY()), m_width, m_height};
    m_srcRect2 = {0, 0, 0, m_height};
    m_dstRect2 = {int(m_position.getX()) + m_width, int(m_position.getY()), 0, m_height};
    m_srcRect1Width = m_dstRect1Width = m_width;
    m_srcRect2Width = m_dstRect2Width = 0;
}

void ScrollingBackground::draw()
{
    SDL_SetTextureAlphaMod(TheTextureManager::Instance()->getTextureMap()[m_textureID], 128);
    // draw first rect
    SDL_RenderCopyEx(RenderContext::get(), TheTextureManager::Instance()->getTextureMap()[m_textureID], &m_srcRect1, &m_dstRect1, 0, 0, SDL_FLIP_NONE);
    // draw second rect
    SDL_RenderCopyEx(RenderContext::get(), TheTextureManager::Instance()->getTextureMap()[m_textureID], &m_srcRect2, &m_dstRect2, 0, 0, SDL_FLIP_NONE);
}

void ScrollingBackground::update()
{
    if(count == maxcount)
    {
        // make first rectangle smaller
        m_srcRect1.x += m_scrollSpeed;
        m_srcRect1.w -= m_scrollSpeed;
        m_dstRect1.w -= m_scrollSpeed;

        // make second rectangle bigger
        m_srcRect2.w += m_scrollSpeed;
        m_dstRect2.w += m_scrollSpeed;
        m_dstRect2.x -= m_scrollSpeed;

        // reset and start again
        if(m_dstRect2.w >= m_width)
        {
            m_srcRect1 = {0, 0, m_width, m_height};
            m_dstRect1 = {int(m_position.getX()), int(m_position.getY()), m_width, m_height};
            m_srcRect2 = {0, 0, 0, m_height};
            m_dstRect2 = {int(m_position.getX()) + m_width, int(m_position.getY()), 0, m_height};
            m_srcRect1Width = m_dstRect1Width = m_width;
            m_srcRect2Width = m_dstRect2Width = 0;
        }
        count = 0;
    }
    count++;
}

void ScrollingBackground::clean()
{
    SDLGameObject::clean();
}
