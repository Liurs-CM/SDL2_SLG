#include "AnimatedGraphic.h"
#include <stdexcept>

AnimatedGraphic::AnimatedGraphic(uint8_t numFrames, uint8_t animSpeed) : 
    m_numFrames(numFrames), 
    m_animSpeed(animSpeed), 
    m_timeFrame(1000 / m_animSpeed), 
    m_currentFrame(0) {
        if(m_animSpeed <= 0) {
            throw std::invalid_argument("Animation speed must be positive.");
        }
    }

void AnimatedGraphic::update()
{
    m_animTimer += DELAY_TIME_MS;
    if(m_animTimer >= m_timeFrame)
    { 
        m_animTimer -= m_timeFrame;
        m_currentFrame = (m_currentFrame + 1) % m_numFrames;
    }
}

