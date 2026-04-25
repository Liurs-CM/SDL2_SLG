#ifndef __TileLayer__
#define __TileLayer__
#include "Layer.h"
#include "Level.h"
#include "Vector2D.h"
#include <iostream>
#include <vector>

class TileLayer : public Layer
{
    public:
        TileLayer(int tileSize, const std::vector<Tileset>& tilesets);
        virtual ~TileLayer() {}
        virtual void update(Level* pLevel);
        virtual void render();
        void setTileIDs(const std::vector<std::vector<int>>& data) { m_tileIDs = data; }
        void setTileSize(int tileSize) { m_tileSize = tileSize; }
        void setMapWidth(int mapWidth) { m_mapWidth = mapWidth; }
        void setPosition(Vector2D position) { m_position = position; }
        int getTileSize() { return m_tileSize; }
        const std::vector<std::vector<int>>& getTileIDs() { return m_tileIDs; }
        Tileset getTilesetByID(int tileID);
        const Vector2D getPosition() { return m_position; }
    private:
        int m_numColumns;
        int m_numRows;
        int m_mapWidth;
        int m_tileSize;
        const std::vector<Tileset>& m_tilesets;
        Vector2D m_position;
        Vector2D m_velocity;
        std::vector<std::vector<int>> m_tileIDs;
};
#endif // __TileLayer__
