#include "Level1Boss.h"
#include "SoundManager.h"

void Level1Boss::update()
{
    if(!m_entered)
    {
        if(m_position.x < (SCR_W - (m_width + 20)))
        {
            m_entered = true;
            std::cout << "boss enter pos: " << m_position << "\n";
        }
    }
    else
    {
        SDLGameObject::update();
        if(m_position.y + m_height >= SCR_H)
        {
            m_velocity.y = -1;
        }
        else if(m_position.y <= 0)
        {
            m_velocity.y = 1;
        }
    }
}
