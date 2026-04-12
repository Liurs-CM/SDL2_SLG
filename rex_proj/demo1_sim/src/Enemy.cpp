#include "GameLib.h"
#include "Enemy.h"
#include "Game.h"
#include "InputHandler.h"
#include "RenderContext.h"

Enemy::Enemy() : SDLGameObject() {}

void Enemy::draw()
{
    TextureManager::Instance()->drawFrame(m_textureID, (Uint32)m_position.getX(), (Uint32)m_position.getY(), m_width, m_height, m_currentRow, m_currentFrame, SDL_FLIP_HORIZONTAL);

    //TheTextureManager::Instance()->drawPrintf(10, 210, COLOR_YELLOW, "Fram: %d; Speed: %.2f; Texture: %s", m_currentFrame, m_animSpeed, m_textureID);
    //SDL_SetRenderDrawColor(RenderContext::get(), 255,0,255,255);
}

void Enemy::update()
{
    m_position -= Vector2D(0.2,0);
	handleInput();
    SDLGameObject::update();
}

void Enemy::clean() { }

void Enemy::handleInput() { }

void Enemy::load(const LoaderParams *pParams)
{
    SDLGameObject::load(pParams);
}

void Enemy::SetPosition(Vector2D pos) 
{ 
    m_position = pos; 
    //m_position = pos + Vector2D(24, 24); 
}

bool Enemy::isDead()
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

std::unique_ptr<SDLGameObject> Enemy::clone()
{
    // 返回当前对象的完整副本（包括纹理、宽高等）
    return std::make_unique<Enemy>(*this);  // 调用拷贝构造
}
