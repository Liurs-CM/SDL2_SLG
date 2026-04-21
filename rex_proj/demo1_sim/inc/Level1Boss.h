#ifndef __Level1Boss__
#define __Level1Boss__
#include "Enemy.h"
#include "GameObjectFactory.h"

class Level1Boss : public Enemy
{
    public:
        Level1Boss() : Enemy() {
            m_health = 100;
            m_dyingTime = 100;
            m_bulletFiringSpeed = 100;
            m_moveSpeed = 2;
            m_entered = false;
        }
        virtual ~Level1Boss() {}
        virtual void load(std::unique_ptr<LoaderParams> const &pParams)
        {
            SDLGameObject::load(std::move(pParams));
            m_velocity.setY(-m_moveSpeed);
        }
        void collision();
        void update();
    private:
        bool m_entered;
};


class Level1BossCreator : public BaseCreator 
{
    GameObject* createGameObject() const
    {
        return new Level1Boss();
    }
};


#endif // __Level1Boss__
