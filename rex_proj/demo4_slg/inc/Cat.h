#ifndef __Cat__
#define __Cat__
#include "Enemy.h"

class Cat : public Enemy
{
    public:
        virtual ~Cat() {}
        Cat() : Enemy() {
            m_health = 1;
        }
        virtual void load(std::unique_ptr<LoaderParams> const &pParams)
        {
            SDLGameObject::load(std::move(pParams));
        }
        virtual void update()
        {
            if(m_position.getX() > (SCR_W - m_width - 10)) {
                m_velocity.setX(-0.2);
            }
            else if(m_position.getX() < 20) {
                m_velocity.setX(0.2);
            }
            else {
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

#endif // __Cat__
