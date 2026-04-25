#ifndef __AnimatedGraphic__
#define __AnimatedGraphic__
#include "GameObjectFactory.h"
#include "SDLGameObject.h"

class AnimatedGraphic : public SDLGameObject
{
    public:
        AnimatedGraphic();
        ~AnimatedGraphic() {}
        virtual void draw();
        virtual void update();
        virtual void clean();
		virtual void load(const LoaderParams* pParams);
    private:
        int m_animSpeed;
        int m_frameCount;
};

class AnimatedGraphicCreator : public BaseCreator
{
    public:
        virtual GameObject* createGameObject() const
        {
            return new AnimatedGraphic();
        }
};

#endif // __AnimatedGraphic__
