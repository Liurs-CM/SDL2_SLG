#include "SDLGameObject.h"
#include "TextureManager.h"
#include "Game.h"
#include "GameLib.h"

SDLGameObject::SDLGameObject() : GameObject(), 
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
    m_currentFrame = m_currentAnim.getCurrentFrame();
}

