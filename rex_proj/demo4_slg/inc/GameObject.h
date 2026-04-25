#ifndef __GameObject__
#define __GameObject__
#include "LoaderParams.h"
#include "Vector2D.h"
#include <string>
#include <memory>

class GameObject
{
    public:
        virtual ~GameObject() {}
        virtual void load(std::unique_ptr<LoaderParams> const &pParams)=0;
        virtual void draw()=0;
        virtual void update()=0;
        virtual void clean()=0;
        virtual void collision() = 0;
        virtual std::string type() = 0;
        Vector2D& getPosition() { return m_position; }
        Vector2D& getVelocity() { return m_velocity; }
        uint16_t getWidth() { return m_width; }
        uint16_t getHeight() { return m_height; }
        void scroll(float scrollSpeed)
        {
            if(type() != std::string("Player")) // player is never scrolled
            {
                m_position.setX(m_position.getX() - scrollSpeed);
            }
        }
        bool updating() { return m_bUpdating; }
        bool dead() { return m_bDead; }
        bool dying() { return m_bDying; }
        void setUpdating(bool updating) { m_bUpdating = updating; }
    protected:
        GameObject() :  
            m_position(0,0),
            m_velocity(0,0),
            m_acceleration(0,0),
            m_width(0),
            m_height(0),
            m_currentRow(1),
            m_currentFrame(0),
            m_bUpdating(false),
            m_bDead(false),
            m_bDying(false),
            m_angle(0),
            m_alpha(255) { }
        Vector2D m_position;
        Vector2D m_velocity;
        Vector2D m_acceleration;
        uint16_t m_width;
        uint16_t m_height;
        uint8_t m_currentRow;
        uint8_t m_currentFrame;
        uint8_t m_numFrames;
        uint8_t m_animSpeed;
        std::string m_textureID;
        bool m_bUpdating;
        bool m_bDead;
        bool m_bDying;
        double m_angle;
        int m_alpha;
};

#endif // __GameObject__
