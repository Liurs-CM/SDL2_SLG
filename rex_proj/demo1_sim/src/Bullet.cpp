#include "GameLib.h"
#include "Bullet.h"
#include "InputHandler.h"
#include "TextureManager.h"

Bullet::Bullet() : SDLGameObject() {}

void Bullet::draw()
{
    //SDLGameObject::draw();
    TextureManager::Instance()->drawRotat(m_textureID, (Uint32)m_position.getX(), (Uint32)m_position.getY(), m_width, m_height, m_angle);
}

void Bullet::update()
{
	handleInput();
    m_acceleration = Vector2D(0.001 * m_velocity.getY(),  -0.001 * m_velocity.getX());
    m_angle = std::atan2(m_velocity.getY(), m_velocity.getX()) * 180.0 / M_PI;
    SDLGameObject::update();
}

void Bullet::clean() { }

void Bullet::handleInput() { }

void Bullet::load(const LoaderParams *pParams)
{
    SDLGameObject::load(pParams);
    m_velocity = Vector2D(0.1,0);
    //m_position = Vector2D(0.1,0);
    //m_acceleration = Vector2D(0.0002, 0);
}

void Bullet::SetPosition(Vector2D pos) 
{ 
    //m_position = pos; 
    //m_position = pos + Vector2D(24, 24); 
    m_position = pos + Vector2D(24, 24); 
}

bool Bullet::isDead()
{
    if (m_position.getX() > SCR_W ||
            m_position.getX() < 0 ||
            m_position.getY() > SCR_H ||
            m_position.getY() < 0)
    {
        return true;
    }
    else if(collision_state)
    {
        return true;
    }
    else
    {
        return false;
    }
}

std::unique_ptr<SDLGameObject> Bullet::clone()
{
    // 返回当前对象的完整副本（包括纹理、宽高等）
    return std::make_unique<Bullet>(*this);  // 调用拷贝构造
}
