#ifndef __Enemy__
#define __Enemy__
//#include "Player.h"
#include "SDLGameObject.h"
#include "GameObjectFactory.h"

class Enemy : public SDLGameObject
{
    public:
        Enemy();
        void draw();
        void update();
        void clean();
        void load(const LoaderParams* pParams);
        std::unique_ptr<SDLGameObject> clone();
        void SetPosition(Vector2D pos);
        bool isDead();
        bool collision_state = false;
    private:
        void handleInput();
};

class EnemyCreator : public BaseCreator
{
    public:	
        GameObject* createGameObject() const
        {
            return new Enemy();
        }
};

#endif /* defined(__Enemy__) */
