#pragma once
#include "SDLGameObject.h"
#include "GameObjectFactory.h"

class Player : public SDLGameObject
{
    public:
        Player();
        virtual ~Player() {}
        virtual void load(std::unique_ptr<LoaderParams> const &pParams);
        void draw();
        void update();
        void clean();
        virtual std::string type() { return "Player"; }
        bool handleInput();
    private:
        Direction m_currentDirection = Direction::RIGHT;
        static const char* dirNames[4];
        float shift_speed = 0.5;
        bool moving = false;
        int8_t m_animTimer = 0;
        uint8_t m_delayFrame;
        vec to_position;
        vec2 at_position;
        vec at_positionScreen;
        void handleAnimation();
};

class PlayerCreator : public BaseCreator
{
    public:	
        GameObject* createGameObject() const
        {
            return new Player();
        }
};

