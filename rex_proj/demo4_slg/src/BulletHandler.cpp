#include "BulletHandler.h"
#include "Game.h"

BulletHandler* BulletHandler::s_pInstance = new BulletHandler();

BulletHandler::BulletHandler() { }

void BulletHandler::addBullet(int x, int y, int width, int height, std::string textureID, int numFrames, Vector2D heading)
{
    auto pBullet = std::make_unique<Bullet>();
    pBullet->load(std::make_unique<LoaderParams>(x, y, width, height, textureID, numFrames), heading);
    m_Bullets.push_back(std::move(pBullet));
}

void BulletHandler::clearBullets()
{
    m_Bullets.clear();
}

void BulletHandler::updateBullets()
{
    for (auto p_it = m_Bullets.begin(); p_it != m_Bullets.end();)
    {
        if((*p_it)->outScreen() || (*p_it)->dead())
        {
            p_it = m_Bullets.erase(p_it);
        }
        else
        {
            (*p_it)->update();
            ++p_it;
        }
    }
}

void BulletHandler::drawBullets()
{
    GameLib::forEachCall(m_Bullets, &Bullet::draw);
}

