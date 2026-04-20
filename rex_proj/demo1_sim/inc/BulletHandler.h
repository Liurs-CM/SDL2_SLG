#ifndef __BulletHandler__
#define __BulletHandler__
#include "Bullet.h"
#include <iostream>
#include <memory>
#include <vector>

class BulletHandler
{
    public:
        static BulletHandler* Instance()
        {
            if(s_pInstance == 0)
            {
                s_pInstance = new BulletHandler();
                return s_pInstance;
            }
            return s_pInstance;
        }
        void addPlayerBullet(int x, int y, int width, int height, std::string textureID, int numFrames, Vector2D heading);
        void addEnemyBullet(int x, int y, int width, int height, std::string textureID, int numFrames, Vector2D heading);
        void updateBullets();
        void drawBullets();
        void clearBullets();
        const std::vector<std::unique_ptr<PlayerBullet>>& getPlayerBullets() const { 
            return m_playerBullets; 
        }
        const std::vector<std::unique_ptr<EnemyBullet>>& getEnemyBullets() const { 
            return m_enemyBullets; 
        }
    private:
        BulletHandler();
        ~BulletHandler();
        BulletHandler(const BulletHandler&);
        BulletHandler& operator=(const BulletHandler&);
        static BulletHandler* s_pInstance;
        // in play bullets
        std::vector<std::unique_ptr<PlayerBullet>> m_playerBullets;
        std::vector<std::unique_ptr<EnemyBullet>> m_enemyBullets;
};

typedef BulletHandler TheBulletHandler;

#endif // __BulletHandler__
