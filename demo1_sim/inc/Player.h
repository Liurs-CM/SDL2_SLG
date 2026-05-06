#ifndef __Player__
#define __Player__
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
        virtual void collision();
        virtual std::string type() { return "Player"; }
        enum class Direction { NORMAL = 0, UP = 1, DOWN = 2, LEFT = 3, RIGHT = 4 };
    private:
        void ressurect();
        void handleInput();
        Direction m_currentDirection = Direction::RIGHT;
        void handleAnimation();
        static const char* dirNames[4];
        float speed = 0.5;
        bool moving = false;
        float animTimer = 0;
        Vector2D to_position;
        Vector2D at_position;
};

class PlayerCreator : public BaseCreator
{
    public:	
        GameObject* createGameObject() const
        {
            return new Player();
        }
};

#endif /* defined(__Player__) */
