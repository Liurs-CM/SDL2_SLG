#include "SDLGameObject.h"
#include "TextureManager.h"
#include "Camera.h"
#include "GameLib.h"

SDLGameObject::SDLGameObject() :  GameObject(), 
    m_position(0,0),
    m_velocity(0,0),
    m_acceleration(0,0),
    m_angle(0),
    m_alpha(255),
    m_bPlayedDeathSound(false)
{ }

void SDLGameObject::load(std::unique_ptr<LoaderParams> const &pParams)
{
    m_position = vec(pParams->getX(), pParams->getY());
    m_width = pParams->getWidth();
    m_height = pParams->getHeight();
    m_textureID = pParams->getTextureID();
    m_animClip.m_numFrames = std::max<uint8_t>(1, pParams->getNumFrames());
    m_animClip.m_animSpeed = std::max<uint8_t>(1, pParams->getAnimSpeed());
    m_animClip.m_delayFrame = FPS / m_animClip.m_animSpeed;
}

void SDLGameObject::draw()
{
    if(m_velocity.x >= 0) {
        TextureManager::Instance()->drawFrame(m_textureID, m_positionScreen, m_width, m_height, m_animClip.m_currentRow, m_animClip.m_currentFrame);
    } else {
        TextureManager::Instance()->drawFrame(m_textureID, m_positionScreen, m_width, m_height, m_animClip.m_currentRow, m_animClip.m_currentFrame, 255, SDL_FLIP_HORIZONTAL);
    }
    TextureManager::Instance()->drawHealthBar(m_positionScreen + vec(4,  -1 ), CELL_SIZE - 8, 4, 5 * m_animClip.m_currentAnim.globalFrame_, 1000);
    //drawHealthBar(204, 104, 20, 4, 500, 1000);
    //drawHealthBar(204, 128, 50, 5, 5 * m_currentAnim.globalFrame_, 1000);
    //drawHealthBar(204, 152, 80, 8, 1000 - 5 * m_currentAnim.globalFrame_, 1000);
}

void SDLGameObject::update()
{
    m_velocity += m_acceleration;
    m_position += m_velocity;
    m_positionScreen = m_position - TheCamera::Instance()->getPosition();
    m_animClip.handleAnimation();
    //m_currentFrame = m_currentAnim.getCurrentFrame(m_animSpeed, m_numFrames);
}

void AnimationClip::handleAnimation()
{
    m_animTimer++;
    if(m_animTimer >= m_delayFrame)
    {
        m_animTimer -= m_delayFrame;
        if (m_currentAction == UnitAction::MOVING) {
            m_currentFrame = (m_currentFrame >= m_numFrames-2) ? 0xff : m_currentFrame;
            m_currentFrame += 1;
            m_currentFrame = (m_currentFrame == 0xff) ? m_numFrames-1 : m_currentFrame;
        } else {
            m_currentFrame = (m_currentFrame == 0) ? 3 : 0; // standing frame
        }
    }
}

