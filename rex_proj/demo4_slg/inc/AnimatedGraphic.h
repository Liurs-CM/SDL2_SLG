#ifndef __AnimatedGraphic__
#define __AnimatedGraphic__
#include "GameLib.h"
#include "SDL.h"

class AnimatedGraphic
{
    public:
        AnimatedGraphic() {}
        ~AnimatedGraphic() {}
        static inline uint8_t globalFrame_ = 0;
        static void updateGlobalFrame() { globalFrame_ = (globalFrame_ + 1) % FPS; }
        uint8_t getCurrentFrame(uint8_t m_delayFrame, uint8_t m_numFrames) {
            return  ( globalFrame_ / m_delayFrame) % m_numFrames;
        }
};

#endif // __AnimatedGraphic__
