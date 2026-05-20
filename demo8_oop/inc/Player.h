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
    private:
        enum class Direction { NORMAL = 0, UP = 1, DOWN = 2, LEFT = 3, RIGHT = 4 };
        Direction m_currentDirection = Direction::RIGHT;
        static const char* dirNames[4];
        float shift_speed = 0.5;
        bool moving = false;
        int8_t m_animTimer = 0;
        uint8_t m_delayFrame;
        Vector2D to_position;
        Vector2D at_position;
        Vector2D at_positionScreen;
        void handleInput();
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

