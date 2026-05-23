#pragma once
#include "GameObject.h"
#include "LoaderParams.h"
#include "AnimatedGraphic.h"
#include "Vector2D.h"
#include <string>
#include <memory>

struct AnimationClip {
    std::string textureID;      // 纹理名（如 "player_walk"）
    bool loop = false;          // 是否循环（idle 循环，attack 不循环）
    uint8_t m_currentRow = 1;
    uint8_t m_currentFrame = 0;
    uint8_t m_numFrames = 1;
    uint8_t m_animSpeed = 1;
    uint8_t m_delayFrame;
    int8_t m_animTimer = 0;
    UnitAction m_currentAction = UnitAction::IDLE;
    AnimatedGraphic m_currentAnim;
    void handleAnimation();
    UnitAction getCurrentAction() const { return m_currentAction; }
    // 自定义构造函数（可选）
};

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
        void setAction(UnitAction act) { m_animClip.m_currentAction = act; }
    protected:
        SDLGameObject();
        vec2 m_position;
        vec2 m_velocity;
        vec2 m_acceleration;
        double m_angle;
        int m_alpha;
        vec m_positionScreen;
        uint16_t m_width;
        uint16_t m_height;
        bool m_bPlayedDeathSound;
        AnimationClip m_animClip;
};

