#ifndef __Bullet__
#define __Bullet__
#include "SDLGameObject.h"
#include "TextureManager.h"
#include "Game.h"
#include <iostream>

class Bullet : public SDLGameObject
{
    public:
        Bullet() {}
        virtual ~Bullet() {}
        virtual std::string type() { return "Bullet"; }
        virtual void load(std::unique_ptr<LoaderParams> pParams, Vector2D heading)
        {
            SDLGameObject::load(std::move(pParams));
            m_velocity = Vector2D(0.1,0);
            m_heading = heading;
        }
        virtual void draw()
        {
            //SDLGameObject::draw();
            TextureManager::Instance()->drawRotat(m_textureID, (Uint32)m_position.getX(), (Uint32)m_position.getY(), m_width, m_height, m_angle);
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
                //m_velocity.setX(m_heading.getX());
                //m_velocity.setY(m_heading.getY());
                m_acceleration = Vector2D(0.001 * m_velocity.getY(),  -0.001 * m_velocity.getX());
                m_angle = std::atan2(m_velocity.getY(), m_velocity.getX()) * 180.0 / M_PI;
                SDLGameObject::update();
            }
            else
            {
                m_velocity.setX(0);
                m_velocity.setY(0);
                //doDyingAnimation();
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
        void SetPosition(Vector2D pos) 
        { 
            m_position = pos + Vector2D(24, 24); 
        }
        std::unique_ptr<SDLGameObject> clone()
        {
            // 返回当前对象的完整副本（包括纹理、宽高等）
            return std::make_unique<Bullet>(*this);  // 调用拷贝构造
        }
    private:
        Vector2D m_heading;
};

#endif // __Bullet__
