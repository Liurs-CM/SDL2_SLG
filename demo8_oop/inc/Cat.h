#pragma once
#include "Enemy.h"
#include "GameObjectFactory.h"

class Cat : public Enemy
{
    public:
        virtual ~Cat() {}
        Cat() : Enemy() { m_health = 1; }
        virtual void load(std::unique_ptr<LoaderParams> const &pParams);
        void update();
        void set2Pos(vec Pos, bool to = true);
        bool move();
    private:
        vec to_position;
        bool movestart = true;
};

class CatCreator : public BaseCreator
{
    GameObject* createGameObject() const
    {
        return new Cat();
    }
};

