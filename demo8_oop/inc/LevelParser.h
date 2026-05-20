#pragma once
#include "lib/tinyxml2.h"
#include <string>
#include <vector>
using namespace tinyxml2;

class Level;
struct Tileset;
class Layer;
class TileLayer;

class LevelParser
{
    public:
        Level* parseLevel(const char* stateFile);
    private:
        void parseTilesets(XMLElement* pTilesetRoot, std::vector<Tileset>* pTilesets);
        void parseTileLayer(XMLElement* pTileElement, std::vector<Layer*> *pLayers, const std::vector<Tileset>* pTilesets, std::vector<TileLayer*> *pCollisionLayers);
        void parseObjectProperties(XMLElement* objectElement, int& width, int& height, int& numFrames, std::string& textureID, int& callbackID, int& animSpeed);
        void parseObjectLayer(XMLElement* pObjectElement, std::vector<Layer*> *pLayers, Level* pLevel);
        void parseTextures(XMLElement* pTextureRoot);
        int m_tileSize;
        int m_width;
        int m_height;
};


