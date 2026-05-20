#pragma once
#include "Enemy.h"

class Cat : public Enemy
{
    public:
        virtual ~Cat() {}
        Cat() : Enemy() { m_health = 1; }
        virtual void load(std::unique_ptr<LoaderParams> const &pParams)
        {
            SDLGameObject::load(std::move(pParams));
            m_velocity.setX(0.1);
            //std::cout << "Cat size: " << m_height << "," << m_width << "\n";
            //std::cout << "Cat speed: " << m_velocity << "\n";
        }
        virtual void update()
        {
            if(m_position.getX() > (SCR_W - m_width) ) {
                m_velocity.setX(-0.2);
            }
            else if(m_position.getX() < 0) {
                m_velocity.setX(0.2);
            }
            SDLGameObject::update();
        }
};

class CatCreator : public BaseCreator
{
    GameObject* createGameObject() const
    {
        return new Cat();
    }
};

