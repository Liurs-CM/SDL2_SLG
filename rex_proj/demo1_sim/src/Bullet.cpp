#include "GameLib.h"
#include "Bullet.h"
#include "InputHandler.h"

Bullet::Bullet() : SDLGameObject() {}

void Bullet::draw()
{
    SDLGameObject::draw();
}

void Bullet::update()
{
	handleInput();
    SDLGameObject::update();
}

void Bullet::clean() { }

void Bullet::handleInput() { }

void Bullet::load(const LoaderParams *pParams)
{
    SDLGameObject::load(pParams);
    m_position = Vector2D(0.1,0);
    m_acceleration = Vector2D(0.0002, 0);
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
