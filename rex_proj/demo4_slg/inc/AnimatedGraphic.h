#ifndef __AnimatedGraphic__
#define __AnimatedGraphic__
#include "GameLib.h"
#include "SDL.h"

class AnimatedGraphic
{
    public:
        AnimatedGraphic(uint8_t numFrames = 1, uint8_t animSpeed = 4);
        ~AnimatedGraphic() {}
        void update();
        uint8_t getCurrentFrame() const { return m_currentFrame; }
        uint16_t getFrameTime() const { return m_timeFrame; }
    private:
        uint8_t m_numFrames;
        uint8_t m_animSpeed;
        uint16_t m_timeFrame;
        uint16_t m_animTimer;
        uint8_t m_currentFrame;
};

#endif // __AnimatedGraphic__
