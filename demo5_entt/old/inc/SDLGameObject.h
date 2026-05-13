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
        virtual std::string type() { return "SDLGameObject"; }
        Vector2D& getPosition() { return m_position; }
        Vector2D& getVelocity() { return m_velocity; }
        const uint16_t getWidth() { return m_width; }
        const uint16_t getHeight() { return m_height; }
        virtual std::unique_ptr<SDLGameObject> clone() { return nullptr; }
    protected:
        SDLGameObject();
        uint8_t m_currentRow;
        uint8_t m_currentFrame;
        double m_angle;
        int m_alpha;
        Vector2D m_positionScreen;
        uint16_t m_width;
        uint16_t m_height;
        uint8_t m_numFrames;
        uint8_t m_animSpeed;
        bool m_bPlayedDeathSound;
        AnimatedGraphic m_currentAnim;
};

#endif /* defined(__SDLGameObject__) */
