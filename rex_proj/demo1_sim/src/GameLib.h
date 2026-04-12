#ifndef __GameLib__
#define __GameLib__

#include <stdlib.h>

#define FPS   165
#define DELAY_TIME  (FPS / 1000)
#define SCR_W   640
#define SCR_H   480
#define SCR_X   1000
#define SCR_Y   500

// Color helper macros
#define COLOR_RGB(r, g, b)     ((uint32_t)(0xFF000000 | (((r) & 0xFF) << 16) | (((g) & 0xFF) << 8) | ((b) & 0xFF)))
#define COLOR_ARGB(a, r, g, b) ((uint32_t)((((a) & 0xFF) << 24) | (((r) & 0xFF) << 16) | (((g) & 0xFF) << 8) | ((b) & 0xFF)))

// Color component extraction
#define COLOR_GET_A(c)    (((c) >> 24) & 0xFF)
#define COLOR_GET_R(c)    (((c) >> 16) & 0xFF)
#define COLOR_GET_G(c)    (((c) >> 8) & 0xFF)
#define COLOR_GET_B(c)    ((c) & 0xFF)

class GameLib
{
    public:
        GameLib();
        ~GameLib();
        // -------- Helper Functions --------
        static int Random(int minVal, int maxVal);

    private:
        // disable copy
        GameLib(const GameLib &);
        GameLib &operator=(const GameLib &);
};


#endif /* defind(__GameLib__) */
