#ifndef __SDLGameObject__
#define __SDLGameObject__
#include "GameObject.h"
#include "LoaderParams.h"
#include "Vector2D.h"
#include <string>
#include <memory>

class SDLGameObject : public GameObject
{
    public:
        virtual ~SDLGameObject() {} //for polymorphism
        virtual void load(std::unique_ptr<LoaderParams> const &pParams);
        virtual void draw();
        virtual void update();
        virtual void clean() {}
        virtual void collision() {}
        virtual std::string type() { return "SDLGameObject"; }
        virtual std::unique_ptr<SDLGameObject> clone() { return nullptr; }
    protected:
        SDLGameObject();
        float m_animSpeed;
        void doDyingAnimation();
        int m_bulletFiringSpeed;
        int m_bulletCounter;
        int m_moveSpeed;
        int m_dyingTime;
        int m_dyingCounter;
        bool m_bPlayedDeathSound;
};

#endif /* defined(__SDLGameObject__) */
