#ifndef __Level__
#define __Level__
#include "Layer.h"
#include "LevelParser.h"
#include "Player.h"
#include <iostream>
#include <vector>

class TileLayer;

struct Tileset
{
    int firstGridID;
    int tileWidth;
    int tileHeight;
    int spacing;
    int margin;
    int width;
    int height;
    int numColumns;
    std::string name;
};

class Level
{
    public:
        ~Level();
        void update();
        void render();
        std::vector<Tileset>* getTilesets() { return &m_tilesets; }
        std::vector<Layer*>* getLayers() { return &m_layers; }    
        std::vector<TileLayer*>* getCollisionLayers() { return &m_collisionLayers; }
        const std::vector<TileLayer*>& getCollidableLayers() { return m_collisionLayers; }
        Player* getPlayer() { return m_pPlayer; }
        void setPlayer(Player* pPlayer) { m_pPlayer = pPlayer; }
    private:
        friend class LevelParser;
        Level();
        std::vector<Tileset> m_tilesets;
        std::vector<Layer*> m_layers;
        std::vector<TileLayer*> m_collisionLayers;
        Player* m_pPlayer;
};

#endif // __Level__
