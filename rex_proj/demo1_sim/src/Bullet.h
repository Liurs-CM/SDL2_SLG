#ifndef __Bullet__
#define __Bullet__
#include "SDLGameObject.h"
#include "GameObjectFactory.h"
class Player;

class Bullet : public SDLGameObject
{
    public:
        Bullet();
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

class BulletCreator : public BaseCreator
{
    public:	
        GameObject* createGameObject() const
        {
            return new Bullet();
        }
};

#endif /* defined(__Bullet__) */
