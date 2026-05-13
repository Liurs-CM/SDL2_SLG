#ifndef __Shape__
#define __Shape__
#include "RenderContext.h"
#include "AnimatedGraphic.h"

class Shape
{
    public:
        void initHealthBarSystem();
        void drawHealthBar(
                int x, int y,               // 屏幕位置
                int width, int height,      // 血条尺寸
                int currentHP, int maxHP);   // 绝对血量值
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
