#pragma once
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
        vec2& getPosition() { return m_position; }
        vec2& getVelocity() { return m_velocity; }
        const uint16_t getWidth() { return m_width; }
        const uint16_t getHeight() { return m_height; }
        virtual std::unique_ptr<SDLGameObject> clone() { return nullptr; }
    protected:
        SDLGameObject();
        vec2 m_position;
        vec2 m_velocity;
        vec2 m_acceleration;
        uint8_t m_currentRow;
        uint8_t m_currentFrame;
        double m_angle;
        int m_alpha;
        vec m_positionScreen;
        uint16_t m_width;
        uint16_t m_height;
        uint8_t m_numFrames;
        uint8_t m_animSpeed;
        bool m_bPlayedDeathSound;
        AnimatedGraphic m_currentAnim;
};

