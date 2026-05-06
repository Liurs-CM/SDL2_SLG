#include "SDLGameObject.h"
#include "TextureManager.h"
#include "Game.h"
#include "GameLib.h"

SDLGameObject::SDLGameObject() : GameObject(), 
    m_bulletFiringSpeed(0),
    m_bulletCounter(0),
    m_moveSpeed(0),
    m_dyingTime(0),
    m_dyingCounter(0),
    m_bPlayedDeathSound(false) {}

void SDLGameObject::load(std::unique_ptr<LoaderParams> const &pParams)
{
    m_position = Vector2D(pParams->getX(), pParams->getY());
    m_width = pParams->getWidth();
    m_height = pParams->getHeight();
    m_textureID = pParams->getTextureID();
    m_numFrames = std::max(1, pParams->getNumFrames());
    m_animSpeed = pParams->getAnimSpeed();
}

void SDLGameObject::draw()
{
    if(m_velocity.getX() >= 0) {
        TextureManager::Instance()->drawFrame(m_textureID, (Uint32)m_position.getX(), (Uint32)m_position.getY(), m_width, m_height, m_currentRow, m_currentFrame);
    } else {
        TextureManager::Instance()->drawFrame(m_textureID, (Uint32)m_position.getX(), (Uint32)m_position.getY(), m_width, m_height, m_currentRow, m_currentFrame, SDL_FLIP_HORIZONTAL);
    }
}

void SDLGameObject::update()
{
    m_velocity += m_acceleration;
    m_position += m_velocity;
    m_currentFrame = int((m_animSpeed * SDL_GetTicks() / 100.0f)) % m_numFrames;
}

void SDLGameObject::doDyingAnimation()
{
    m_currentFrame = int((m_animSpeed * SDL_GetTicks() / 100.0f)) % m_numFrames;
    if(m_dyingCounter == m_dyingTime) {
        m_bDead = true;
    }
    m_dyingCounter++;
}
