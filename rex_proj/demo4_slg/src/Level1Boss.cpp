#include "Level1Boss.h"
#include "SoundManager.h"
#include "BulletHandler.h"

void Level1Boss::collision()
{
    if(m_entered)
    {
        m_health -= 1;
        if(m_health == 0)
        {
            if(!m_bPlayedDeathSound)
            {
                m_position.setX(m_position.getX() + 30);
                m_position.setY(m_position.getY() + 70);
                TheSoundManager::Instance()->playSound("explode", 0);
                m_textureID = "bossexplosion";
                m_currentFrame = 0;
                m_numFrames = 9;
                m_width = 180;
                m_height = 180;
                m_bDying = true;
            }
        }
    }
}

void Level1Boss::update()
{
    if(!m_entered)
    {
        scroll(TheGame::Instance()->getScrollSpeed());
        if(m_position.getX() < (SCR_W - (m_width + 20)))
        {
            m_entered = true;
            std::cout << "boss enter pos: " << m_position << "\n";
        }
    }
    else
    {
        if(!m_bDying)
        {
            if(m_position.getY() + m_height >= SCR_H)
            {
                m_velocity.setY(-1);
            }
            else if(m_position.getY() <= 0)
            {
                m_velocity.setY(1);
            }
            SDLGameObject::update();
        }
        else
        {
            scroll(TheGame::Instance()->getScrollSpeed());
            m_currentFrame = int((SDL_GetTicks() / 100) % m_numFrames);
            m_bDead = true;
            TheGame::Instance()->setLevelComplete(true);
        }
    }
}
