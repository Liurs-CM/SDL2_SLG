#ifndef __ScrollingBackground__
#define __ScrollingBackground__
#include "GameObjectFactory.h"
#include "SDLGameObject.h"
#include <iostream>

class ScrollingBackground : public SDLGameObject
{
    public:
        virtual ~ScrollingBackground() {}
        ScrollingBackground();
        virtual void draw();
        virtual void update();
        virtual void clean();
        virtual void load(std::unique_ptr<LoaderParams> const &pParams);
    private:
        int m_scrollSpeed;
        int count;
        int maxcount;
        SDL_Rect m_srcRect1;
        SDL_Rect m_srcRect2;
        SDL_Rect m_dstRect1;
        SDL_Rect m_dstRect2;
        int m_srcRect1Width;
        int m_srcRect2Width;
        int m_dstRect1Width;
        int m_dstRect2Width;
};

class ScrollingBackgroundCreator : public BaseCreator
{
    public:
        virtual GameObject* createGameObject() const
        {
            return new ScrollingBackground();
        }
};

#endif // __ScrollingBackground__
