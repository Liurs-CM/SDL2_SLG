#include "CollisionManager.h"
#include "Player.h"
#include "Enemy.h"
#include "BulletHandler.h"
#include "TileLayer.h"
#include "GameLib.h"

void CollisionManager::checkPlayerEnemyBulletCollision(Player* pPlayer)
{
    SDL_Rect* pRect1 = new SDL_Rect();
    pRect1->x = pPlayer->getPosition().getX();
    pRect1->y = pPlayer->getPosition().getY();
    pRect1->w = pPlayer->getWidth();
    pRect1->h = pPlayer->getHeight();
    for (auto* bullet : TheBulletHandler::Instance()->getEnemyBullets()) 
    {
        SDL_Rect* pRect2 = new SDL_Rect();
        pRect2->x = bullet->getPosition().getX();
        pRect2->y = bullet->getPosition().getY();
        pRect2->w = bullet->getWidth();
        pRect2->h = bullet->getHeight();
        if(RectRectCollision(pRect1, pRect2))
        {
            if(!pPlayer->dying() && !bullet->dying())
            {
                bullet->collision();
                pPlayer->collision();
            }
        }
        delete pRect2;
    }
    delete pRect1;
}

void CollisionManager::checkEnemyPlayerBulletCollision(const std::vector<GameObject *> &objects)
{
    for(auto* pobj : objects)
    {
        for (auto* obj : TheBulletHandler::Instance()->getPlayerBullets()) 
        {
            if(obj->type() != std::string("Enemy") || !obj->updating())
            {
                continue;
            }
            SDL_Rect* pRect1 = new SDL_Rect();
            pRect1->x = pobj->getPosition().getX();
            pRect1->y = pobj->getPosition().getY();
            pRect1->w = pobj->getWidth();
            pRect1->h = pobj->getHeight();
            SDL_Rect* pRect2 = new SDL_Rect();
            pRect2->x = obj->getPosition().getX();
            pRect2->y = obj->getPosition().getY();
            pRect2->w = obj->getWidth();
            pRect2->h = obj->getHeight();
            if(RectRectCollision(pRect1, pRect2))
            {
                if(!pobj->dying() && !obj->dying())
                {
                    obj->collision();
                    pobj->collision();
                }
            }
            delete pRect1;
            delete pRect2;
        }
    }
}

void CollisionManager::checkPlayerEnemyCollision(Player* pPlayer, const std::vector<GameObject*> &objects)
{
    SDL_Rect* pRect1 = new SDL_Rect();
    pRect1->x = pPlayer->getPosition().getX();
    pRect1->y = pPlayer->getPosition().getY();
    pRect1->w = pPlayer->getWidth();
    pRect1->h = pPlayer->getHeight();
    for(auto* obj : objects)
    {
        if(obj->type() != std::string("Enemy") || !obj->updating())
        {
            continue;
        }
        SDL_Rect* pRect2 = new SDL_Rect();
        pRect2->x = obj->getPosition().getX();
        pRect2->y = obj->getPosition().getY();
        pRect2->w = obj->getWidth();
        pRect2->h = obj->getHeight();
        if(RectRect(pRect1, pRect2))
        {
            if(!obj->dead() && !obj->dying())
            {
                pPlayer->collision();
                obj->collision();
            }
        }
        delete pRect2;
    }
    delete pRect1;
}

void CollisionManager::checkPlayerTileCollision(Player* pPlayer, const std::vector<TileLayer*>& collisionLayers)
{
    for(std::vector<TileLayer*>::const_iterator it = collisionLayers.begin(); it != collisionLayers.end(); ++it)
    {
        TileLayer* pTileLayer = (*it);
        std::vector<std::vector<int>> tiles = pTileLayer->getTileIDs();
        Vector2D layerPos = pTileLayer->getPosition();
        int x, y, tileColumn, tileRow, tileid = 0;

        x = layerPos.getX() / pTileLayer->getTileSize();
        y = layerPos.getY() / pTileLayer->getTileSize();

        if(pPlayer->getVelocity().getX() >= 0 || pPlayer->getVelocity().getY() >= 0)
        {
            tileColumn = ((pPlayer->getPosition().getX() + pPlayer->getWidth()) / pTileLayer->getTileSize());
            tileRow = ((pPlayer->getPosition().getY() + pPlayer->getHeight()) / pTileLayer->getTileSize());
            tileid = tiles[tileRow + y][tileColumn + x];
        }
        else if(pPlayer->getVelocity().getX() < 0 || pPlayer->getVelocity().getY() < 0)
        {
            tileColumn = pPlayer->getPosition().getX() / pTileLayer->getTileSize();
            tileRow = pPlayer->getPosition().getY() / pTileLayer->getTileSize();
            tileid = tiles[tileRow + y][tileColumn + x];
        }
        if(tileid != 0)
        {
            pPlayer->collision();
        }
    }
}
