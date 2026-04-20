#ifndef __CollisionManager__
#define __CollisionManager__
#include "SDL.h"
#include <iostream>
#include <vector>

class Player;
class GameObject;
class TileLayer;

class CollisionManager
{
    public:
        void checkPlayerEnemyBulletCollision(Player* pPlayer);
        void checkPlayerEnemyCollision(Player* pPlayer, const std::vector<GameObject*> &objects);
        void checkEnemyPlayerBulletCollision(const std::vector<GameObject*>& objects);
        void checkPlayerTileCollision(Player* pPlayer, const std::vector<TileLayer*>& collisionLayers);
        const static int s_buffer = 4;
        static bool RectRectCollision(SDL_Rect* A, SDL_Rect* B)
        {
            int aHBuf = A->h / s_buffer;
            int aWBuf = A->w / s_buffer;
            int bHBuf = B->h / s_buffer;
            int bWBuf = B->w / s_buffer;
            if((A->y + A->h) - aHBuf <= B->y + bHBuf)  { return false; }
            if(A->y + aHBuf >= (B->y + B->h) - bHBuf)  { return false; }
            if((A->x + A->w) - aWBuf <= B->x +  bWBuf) { return false; }
            if(A->x + aWBuf >= (B->x + B->w) - bWBuf)  { return false; }
            return true;
        }
};

#endif // __CollisionManager__
