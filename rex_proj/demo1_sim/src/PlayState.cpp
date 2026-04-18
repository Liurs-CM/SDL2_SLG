#include "PlayState.h"
#include "MenuPauseState.h"
#include "TextureManager.h"
#include "Game.h"
#include "GameObject.h"
#include "InputHandler.h"
#include "LoaderParams.h"
#include "StateParser.h"
#include <string>
#include <algorithm>
#include <iostream>
//#include "LevelParser.h"
//#include "Level.h"

const std::string PlayState::s_playID = "PLAY";

void PlayState::update()
{
    if(TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
    {
        TheGame::Instance()->getStateMachine()->pushState(new MenuPauseState());
        std::cout << "exit PlayState" << std::endl;
    }
    //else if(TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_SPACE))
    else if(TheInputHandler::Instance()->isKeyPressed(SDL_SCANCODE_SPACE))
    {
        createBullet();
        //std::cout << "bullet num " << m_bullets.size() << std::endl;
    }
    for(auto& obj : m_gameObjects)
    {
        if(dynamic_cast<Player*>(obj.get()) != nullptr)
        {
            obj->update();
        }
    }
    for(const auto& enemy_ptr : m_enemies)
    {
        if(!enemy_ptr) continue;
        SDLGameObject& obj_e = *enemy_ptr;
        for(const auto& bullet_ptr : m_bullets)
        {
            if(!bullet_ptr) continue;
            SDLGameObject& obj_b = *bullet_ptr;
            if( checkCollision(&obj_e, &obj_b))
            {
                enemy_ptr->collision_state = true;
                bullet_ptr->collision_state = true;
                score++;
                //std::cout << "shot the enemy nums:" << score << std::endl;
            }
        }
    }
    if(!m_bullets.empty())
    {
        for(auto& obj : m_bullets)
        {
            obj->update();
            //std::cout << "bullet move " << dbg_num++ << std::endl;
        }
        m_bullets.erase(
                std::remove_if(m_bullets.begin(), m_bullets.end(),
                    [](const std::unique_ptr<Bullet>& b) {
                    return b->isDead();  // 返回 true 表示要删除
                    }),
                m_bullets.end()
                );
    }
    if(!m_enemies.empty())
    {
        for(auto& obj : m_enemies)
        {
            obj->update();
        }
        m_enemies.erase(
                std::remove_if(m_enemies.begin(), m_enemies.end(),
                    [](const std::unique_ptr<Enemy>& b) {
                    return b->isDead();  // 返回 true 表示要删除
                    }),
                m_enemies.end()
                );
    }
    spawnEnemies();
    //pLevel->update();
}

void PlayState::render()
{
    for(auto& obj : m_gameObjects)
    {
        if(dynamic_cast<Player*>(obj.get()) != nullptr)
        {
            obj->draw();
        }
    }
    for(auto& obj : m_bullets)
    {
        obj->draw();
    }
    for(auto& obj : m_enemies)
    {
        obj->draw();
    }
    //pLevel->render();
}

bool PlayState::onEnter()
{
	StateParser stateParser;
	stateParser.parseState("assets/test.xml", s_playID, &m_gameObjects, &m_textureIDList);
    for(auto& obj : m_gameObjects)
    {
        if(dynamic_cast<Player*>(obj.get()) != nullptr)
        {
            player = static_cast<Player*>(obj.get());
        }
    }
    std::cout << "entering PlayState" << std::endl;
    //LevelParser levelParser;
    //pLevel = levelParser.parseLevel("assets/map1.tmx");
    return true;
}

bool PlayState::onExit()
{
    for(auto& obj :  m_gameObjects)
    {
        obj->clean();
    }
    m_gameObjects.clear();
    for(const auto& obj : m_textureIDList)
    {
        TheTextureManager::Instance()->clearFromTextureMap(obj);
    }
    m_bullets.clear();
    m_enemies.clear();
    TheInputHandler::Instance()->reset();
    std::cout << "exiting PlayState" << std::endl;
    return true;
}

bool PlayState::checkCollision(SDLGameObject* p1, SDLGameObject* p2)
{	
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;
    leftA = p1->getPosition().getX();
    rightA = p1->getPosition().getX() + p1->getWidth();
    topA = p1->getPosition().getY();
    bottomA = p1->getPosition().getY() + p1->getHeight();
    // Calculate the sides of rect B
    leftB = p2->getPosition().getX();
    rightB = p2->getPosition().getX() + p2->getWidth();
    topB = p2->getPosition().getY();
    bottomB = p2->getPosition().getY() + p2->getHeight();
    // If any of the sides from A are outside of B
    if(bottomA <= topB) { return false; }
    if(topA >= bottomB) { return false; }
    if(rightA <= leftB) { return false; }
    if(leftA >= rightB) { return false; }
    return true;
}

void PlayState::createBullet()
{
    Bullet* templateBullet = nullptr;
    for(auto& obj : m_gameObjects)
    {
        if(dynamic_cast<Bullet*>(obj.get()) != nullptr)
        {
            templateBullet = static_cast<Bullet*>(obj.get());
        }
    }
    if(templateBullet == nullptr)
    {
        std::cout << "Error: No bullet template found in m_gameObjects!" << std::endl;
        return;
    }
    // 克隆模板对象
    auto newBullet = std::unique_ptr<Bullet>(
        static_cast<Bullet*>(templateBullet->clone().release())
    );
    // 设置新位置
    newBullet->SetPosition(player->getPosition());
    // 添加到容器
    m_bullets.push_back(std::move(newBullet));
}

void PlayState::spawnEnemies()
{
    if (--enemySpawnTimer <= 0)
    {
        enemySpawnTimer = 1000 + (rand() % 60);
        Enemy* templateEnemy = nullptr;
        for(auto& obj : m_gameObjects)
        {
            if(dynamic_cast<Enemy*>(obj.get()) != nullptr)
            {
                templateEnemy = static_cast<Enemy*>(obj.get());
            }
        }
        if(templateEnemy == nullptr)
        {
            std::cout << "Error: No enemy template found in m_gameObjects!" << std::endl;
            return;
        }
        // 克隆模板对象
        auto newEnemy = std::unique_ptr<Enemy>(
                static_cast<Enemy*>(templateEnemy->clone().release())
                );
        // 设置新位置
        newEnemy->SetPosition(Vector2D(SCR_W, rand()%SCR_H));
        // 添加到容器
        m_enemies.push_back(std::move(newEnemy));
    }
}
