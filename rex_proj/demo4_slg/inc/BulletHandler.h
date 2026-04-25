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
        void addBullet(int x, int y, int width, int height, std::string textureID, int numFrames, Vector2D heading);
        void updateBullets();
        void drawBullets();
        void clearBullets();
        const std::vector<std::unique_ptr<Bullet>>& getBullets() const { 
            return m_Bullets; 
        }
    private:
        BulletHandler();
        ~BulletHandler();
        BulletHandler(const BulletHandler&);
        BulletHandler& operator=(const BulletHandler&);
        static BulletHandler* s_pInstance;
        // in play bullets
        std::vector<std::unique_ptr<Bullet>> m_Bullets;
};

typedef BulletHandler TheBulletHandler;

#endif // __BulletHandler__
