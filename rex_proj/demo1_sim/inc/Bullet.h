#ifndef __Bullet__
#define __Bullet__
#include "SDLGameObject.h"
#include "TextureManager.h"
#include "Game.h"
#include <iostream>

class PlayerBullet : public SDLGameObject
{
    public:
        PlayerBullet() : SDLGameObject() {
            m_dyingTime = 5;
    }
        virtual ~PlayerBullet() {}
        virtual std::string type() { return "PlayerBullet"; }
        virtual void load(std::unique_ptr<LoaderParams> pParams, Vector2D heading)
        {
            SDLGameObject::load(std::move(pParams));
            m_heading = heading;
        }
        virtual void draw()
        {
            SDLGameObject::draw();
        }
        virtual void collision()
        {
            m_textureID = "smallexplosion";
            m_currentFrame = 0;
            m_numFrames = 2;
            m_width = 20;
            m_height = 20;
            m_bDying = true;
        }
        virtual void update()
        {
            if(!m_bDying)
            {
                m_velocity.setX(m_heading.getX());
                m_velocity.setY(m_heading.getY());
                SDLGameObject::update();
            }
            else
            {
                m_velocity.setX(0);
                m_velocity.setY(0);
                doDyingAnimation();
            }
        }
        virtual void clean()
        {
            SDLGameObject::clean();
        }
        bool outScreen() 
        {
            if (m_position.getX() > SCR_W ||
                    m_position.getX() < 0 ||
                    m_position.getY() > SCR_H ||
                    m_position.getY() < 0) {
                return true;
            }
            return false;
        }
    private:
        Vector2D m_heading;
};

class EnemyBullet : public PlayerBullet
{
    public:
        EnemyBullet() : PlayerBullet() { }
        virtual ~EnemyBullet() {}
        virtual std::string type() { return "EnemyBullet"; }
};

#endif // __Bullet__
