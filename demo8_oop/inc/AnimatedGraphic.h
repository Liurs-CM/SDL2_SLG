#pragma once
#include "GameLib.h"

// UnitAction.h
enum class UnitAction {
    IDLE,
    MOVING,
    ATTACKING,
    DEAD
};

class AnimatedGraphic
{
    public:
        AnimatedGraphic() {}
        ~AnimatedGraphic() {}
        static inline uint8_t globalFrame_ = 0;
        static void updateGlobalFrame() { globalFrame_ = (globalFrame_ + 1) % FPS; }
        uint8_t getCurrentFrame(uint8_t m_animSpeed, uint8_t m_numFrames) {
            return  ( m_animSpeed * globalFrame_ / FPS) % m_numFrames;
        }
        static inline UnitAction m_currentAction = UnitAction::IDLE;
        static void setSceneAnim(UnitAction act) { m_currentAction = act; }
        UnitAction getSceneAnim() { return m_currentAction ; }
};

