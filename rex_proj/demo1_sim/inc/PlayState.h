#ifndef __PlayState__
#define __PlayState__
#include "GameState.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"
#include "Level.h"
#include <memory>
#include <vector>
//#include "Level.h"

class GameObject;
class SDLGameObject;

class PlayState : public GameState
{
    public:
        virtual ~PlayState() { delete pLevel; }
        virtual void update();
        virtual void render();
        virtual bool onEnter();
        virtual bool onExit();
        virtual std::string getStateID() const { return s_playID; }
        void createBullet();
        void spawnEnemies();
        int dbg_num = 0;
        int enemySpawnTimer = 10;
        int score = 0;
    private:
        static const std::string s_playID;
        std::vector<std::unique_ptr<GameObject>> m_gameObjects;
        Player* player = nullptr;
        std::vector<std::unique_ptr<Bullet>> m_bullets;
        std::vector<std::unique_ptr<Enemy>> m_enemies;
        Level* pLevel;
        bool checkCollision(SDLGameObject* p1, SDLGameObject* p2);
};

#endif // __PlayState__
