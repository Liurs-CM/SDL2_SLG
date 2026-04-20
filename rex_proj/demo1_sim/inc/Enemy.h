#ifndef __Enemy__
#define __Enemy__
#include "SDLGameObject.h"

class Enemy : public SDLGameObject
{
    public:
        bool isDead() { if(m_health <= 0) return true; return false; }
        virtual std::string type() { return "Enemy"; }
    protected:
        int m_health;
        Enemy() : SDLGameObject() {}
        virtual ~Enemy() {}
};

#endif // __Enemy__
