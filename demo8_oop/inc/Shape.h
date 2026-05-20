#ifndef __Shape__
#define __Shape__
#include "RenderContext.h"
#include "AnimatedGraphic.h"

class Shape
{
    public:
        void Shape_genrate();
        static Shape* Instance()
        {
            if(s_pInstance == 0)
            {
                s_pInstance = new Shape();
                return s_pInstance;
            }
            return s_pInstance;
        }

    private:
        Shape() {}
        static Shape* s_pInstance;
        SDL_Renderer* pRenderer = RenderContext::get();
        SDL_Texture* gHealthGradient = nullptr;
        AnimatedGraphic m_currentAnim;
};

typedef Shape TheShape;

#endif /* defined(__Shape__) */
