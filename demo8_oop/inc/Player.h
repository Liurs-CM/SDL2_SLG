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
        void set2Pos(vec Pos, bool to = true);
        bool move();
        //void setToPos(vec Pos) { to_position = Pos; }
        //void setDir(Direction Dir) { m_currentDirection = Dir; }
    private:
        Direction m_currentDirection = Direction::RIGHT;
        static const char* dirNames[4];
        float shift_speed = 0.2;
        bool moving = false;
        vec to_position;
        vec2 at_position;
        vec at_positionScreen;
};

class PlayerCreator : public BaseCreator
{
    public:	
        GameObject* createGameObject() const
        {
            return new Player();
        }
};

