#include "BulletHandler.h"
#include "Game.h"

BulletHandler* BulletHandler::s_pInstance = new BulletHandler();

BulletHandler::BulletHandler() { }

void BulletHandler::addPlayerBullet(int x, int y, int width, int height, std::string textureID, int numFrames, Vector2D heading)
{
    auto pPlayerBullet = std::make_unique<PlayerBullet>();
    pPlayerBullet->load(std::make_unique<LoaderParams>(x, y, width, height, textureID, numFrames), heading);
    m_playerBullets.push_back(std::move(pPlayerBullet));
}

void BulletHandler::addEnemyBullet(int x, int y, int width, int height, std::string textureID, int numFrames, Vector2D heading)
{
    auto pEnemyBullet  = std::make_unique<EnemyBullet>();
    pEnemyBullet->load(std::make_unique<LoaderParams>(x, y, width, height, textureID, numFrames), heading);
    m_enemyBullets.push_back(std::move(pEnemyBullet));
}

void BulletHandler::clearBullets()
{
    m_enemyBullets.clear();
    m_playerBullets.clear();
}

void BulletHandler::updateBullets()
{
    for (auto p_it = m_playerBullets.begin(); p_it != m_playerBullets.end();)
    {
        if((*p_it)->outScreen() || (*p_it)->dead())
        {
            p_it = m_playerBullets.erase(p_it);
        }
        else
        {
            (*p_it)->update();
            ++p_it;
        }
    }

    for (auto e_it = m_enemyBullets.begin(); e_it != m_enemyBullets.end();)
    {
        if((*e_it)->outScreen() || (*e_it)->dead())
        {
            e_it = m_enemyBullets.erase(e_it);
        }
        else
        {
            (*e_it)->update();
            ++e_it;
        }
    }
}

void BulletHandler::drawBullets()
{
    for (auto& obj : m_playerBullets) {
        obj->draw();
    }
    for (auto& obj : m_enemyBullets) {
        obj->draw();
    }
}

