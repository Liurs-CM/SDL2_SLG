#ifndef __SDLGameObject__
#define __SDLGameObject__
#include "GameObject.h"
#include "LoaderParams.h"
#include "AnimatedGraphic.h"
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
        virtual std::unique_ptr<SDLGameObject> clone() { return nullptr; }
        virtual std::string type() { return "SDLGameObject"; }
        Vector2D getPosition() { return m_position; }
        uint16_t getWidth() { return m_width; }
        uint16_t getHeight() { return m_height; }
    protected:
        bool m_bPlayedDeathSound;
        SDLGameObject();
        AnimatedGraphic m_currentAnim;
};

#endif /* defined(__SDLGameObject__) */
