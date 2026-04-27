#include "SDLGameObject.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Game.h"
#include "GameLib.h"

SDLGameObject::SDLGameObject() :  GameObject(), 
    m_position(0,0),
    m_velocity(0,0),
    m_acceleration(0,0),
    m_currentRow(1),
    m_currentFrame(0),
    m_angle(0),
    m_alpha(255),
    m_bPlayedDeathSound(false) 
{ }

void SDLGameObject::load(std::unique_ptr<LoaderParams> const &pParams)
{
    m_position = Vector2D(pParams->getX(), pParams->getY());
    m_width = pParams->getWidth();
    m_height = pParams->getHeight();
    m_textureID = pParams->getTextureID();
    m_numFrames = std::max<uint8_t>(1, pParams->getNumFrames());
    m_animSpeed = std::max<uint8_t>(1, pParams->getAnimSpeed());
}

void SDLGameObject::draw()
{
    if(m_velocity.getX() >= 0) {
        TextureManager::Instance()->drawFrame(m_textureID, m_positionScreen, m_width, m_height, m_currentRow, m_currentFrame);
    } else {
        TextureManager::Instance()->drawFrame(m_textureID, m_positionScreen, m_width, m_height, m_currentRow, m_currentFrame, SDL_FLIP_HORIZONTAL);
    }
}

void SDLGameObject::update()
{
    m_velocity += m_acceleration;
    m_position += m_velocity;
    m_positionScreen = m_position - TheCamera::Instance()->getPosition();
    m_currentFrame = m_currentAnim.getCurrentFrame(m_animSpeed, m_numFrames);
}

