#pragma once
#include "SDLGameObject.h"

class Enemy : public SDLGameObject
{
    public:
        bool isDead() { if(m_health <= 0) return true; return false; }
        virtual std::string type() { return "Enemy"; }
    protected:
        Enemy() : SDLGameObject() {}
        virtual ~Enemy() {}
        int m_health;
};

