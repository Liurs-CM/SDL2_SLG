#include "GameLib.h"
#include "Game.h"
#include "SDLGameObject.h"
#include "TextureManager.h"

SDLGameObject::SDLGameObject() : GameObject(), m_position(0,0), m_velocity(0,0), m_acceleration(0,0) {}

void SDLGameObject::load(const LoaderParams *pParams)
{
    m_position = Vector2D(pParams->getX(), pParams->getY());
    m_velocity = Vector2D(0, 0);
    m_acceleration = Vector2D(0, 0);
    m_width = pParams->getWidth();
    m_height = pParams->getHeight();
    m_textureID = pParams->getTextureID();
    m_currentRow = 1;
    m_currentFrame = 0;
    m_numFrames = std::max(1, pParams->getNumFrames());
    m_animSpeed = pParams->getAnimSpeed();
}

void SDLGameObject::draw()
{
    if(m_velocity.getX() >= 0)
    {
        TextureManager::Instance()->drawFrame(m_textureID, (Uint32)m_position.getX(), (Uint32)m_position.getY(), m_width, m_height, m_currentRow, m_currentFrame);
    }
    else
    {
        TextureManager::Instance()->drawFrame(m_textureID, (Uint32)m_position.getX(), (Uint32)m_position.getY(), m_width, m_height, m_currentRow, m_currentFrame, SDL_FLIP_HORIZONTAL);
    }
}

void SDLGameObject::update()
{
    m_velocity += m_acceleration;
    m_position += m_velocity;
    m_currentFrame = int((m_animSpeed * SDL_GetTicks() / 100.0f)) % m_numFrames;
}
