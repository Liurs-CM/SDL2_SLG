#include "LevelParser.h"
#include "TextureManager.h"
#include "ObjectLayer.h"
#include "TileLayer.h"
#include "GameObjectFactory.h"
#include "Level.h"
#include "lib/base64.h"
#include "lib/zlib.h"
#include <string>

Level* LevelParser::parseLevel(const char *levelFile)
{
    // create a tinyXML document and load the map xml
    XMLDocument levelDocument;
    if(levelDocument.LoadFile(levelFile) != XML_SUCCESS) {
        std::cerr << levelDocument.ErrorStr() << "\n";
    }

    // get the root node and display some values
    XMLElement* pRoot = levelDocument.RootElement();
    std::cout << "Loading level:\n" << "Version: " << pRoot->Attribute("version") << "\n";
    std::cout << "Width:" << pRoot->Attribute("width") << " - Height:" << pRoot->Attribute("height") << "\n";
    std::cout << "Tile Width:" << pRoot->Attribute("tilewidth") << " - Tile Height:" << pRoot->Attribute("tileheight") << "\n";
    pRoot->QueryIntAttribute("tilewidth", &m_tileSize);
    pRoot->QueryIntAttribute("width", &m_width);
    pRoot->QueryIntAttribute("height", &m_height);

    //we know that properties is the first child of the root
    XMLElement* pProperties = pRoot->FirstChildElement();
    // we must parse the textures needed for this level, which have been added to properties
    for(XMLElement* e = pProperties->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
    {
        if(e->Value() == std::string("property")) {
            parseTextures(e);
        }
    }

    // create the level object
    Level* pLevel = new Level();
    // we must now parse the tilesets
    for(XMLElement* e = pRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
        if(e->Value() == std::string("tileset")) {
            parseTilesets(e, pLevel->getTilesets());
        }
    }

    // parse any object layers
    for(XMLElement* e = pRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
        if(e->Value() == std::string("objectgroup") || e->Value() == std::string("layer")) {
            if(e->FirstChildElement()->Value() == std::string("object")) {
                parseObjectLayer(e, pLevel->getLayers(), pLevel);
            }
            else if(e->FirstChildElement()->Value() == std::string("data") ||
                    (e->FirstChildElement()->NextSiblingElement() != 0 && 
                     e->FirstChildElement()->NextSiblingElement()->Value() == std::string("data")))
            {
                parseTileLayer(e, pLevel->getLayers(), pLevel->getTilesets(), pLevel->getCollisionLayers());
            }
        }
    }

    return pLevel;
}


void LevelParser::parseTextures(XMLElement* pTextureRoot)
{
    std::cout << "adding texture " << pTextureRoot->Attribute("value") << " with ID " << pTextureRoot->Attribute("name") << std::endl;
    TheTextureManager::Instance()->load(pTextureRoot->Attribute("value"), pTextureRoot->Attribute("name"));
}

void LevelParser::parseTilesets(XMLElement* pTilesetRoot, std::vector<Tileset>* pTilesets)
{
    std::string assetsTag = "assets/";
    // first add the tileset to texture manager
    std::cout << "adding texture " << pTilesetRoot->FirstChildElement()->Attribute("source") << " with ID " << pTilesetRoot->Attribute("name") << std::endl;
    TheTextureManager::Instance()->load(assetsTag.append(pTilesetRoot->FirstChildElement()->Attribute("source")), pTilesetRoot->Attribute("name"));

    // create a tileset object
    Tileset tileset;
    pTilesetRoot->FirstChildElement()->QueryIntAttribute("width", &tileset.width);
    pTilesetRoot->FirstChildElement()->QueryIntAttribute("height", &tileset.height);
    pTilesetRoot->QueryIntAttribute("firstgid", &tileset.firstGridID);
    pTilesetRoot->QueryIntAttribute("tilewidth", &tileset.tileWidth);
    pTilesetRoot->QueryIntAttribute("tileheight", &tileset.tileHeight);
    pTilesetRoot->QueryIntAttribute("spacing", &tileset.spacing);
    pTilesetRoot->QueryIntAttribute("margin", &tileset.margin);
    tileset.name = pTilesetRoot->Attribute("name");
    tileset.numColumns = tileset.width / (tileset.tileWidth + tileset.spacing);
    pTilesets->push_back(tileset);
}

void LevelParser::parseObjectLayer(XMLElement* pObjectElement, std::vector<Layer*> *pLayers, Level* pLevel)
{
    // create an object layer
    ObjectLayer* pObjectLayer = new ObjectLayer();

    for(XMLElement* e = pObjectElement->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
    {
        if(e->Value() == std::string("object"))
        {
            int x, y, width, height, numFrames, callbackID = 0;
            float animSpeed = 0;
            std::string textureID;
            std::string type;

            // get the initial node values type, x and y
            e->QueryIntAttribute("x", &x);
            e->QueryIntAttribute("y", &y);
            type = e->Attribute("type");
            GameObject* pGameObject = TheGameObjectFactory::Instance()->create(type);

            // get the property values
            for(XMLElement* properties = e->FirstChildElement(); properties != NULL; properties = properties->NextSiblingElement())
            {
                if(properties->Value() == std::string("properties"))
                {
                    for(XMLElement* property = properties->FirstChildElement(); property != NULL; property = property->NextSiblingElement())
                    {
                        if(property->Value() == std::string("property"))
                        {
                            if(property->Attribute("name") == std::string("numFrames")) {
                                property->QueryIntAttribute("value", &numFrames);
                            } else if(property->Attribute("name") == std::string("textureHeight")) {
                                property->QueryIntAttribute("value", &height);
                            } else if(property->Attribute("name") == std::string("textureID")) {
                                textureID = property->Attribute("value");
                            } else if(property->Attribute("name") == std::string("textureWidth")) {
                                property->QueryIntAttribute("value", &width);
                            } else if(property->Attribute("name") == std::string("callbackID")) {
                                property->QueryIntAttribute("value", &callbackID);
                            } else if(e->Attribute("name") == std::string("animSpeed")) {
                                property->QueryFloatAttribute("value", &animSpeed);
                            }
                        }
                    }
                }
            }
            //int x, int y, int width, int height, std::string textureID, int numFrames, void()
            pGameObject->load(new LoaderParams(x, y, width, height, textureID, numFrames,callbackID, animSpeed));
            if(type == "Player") {
                pLevel->setPlayer(dynamic_cast<Player*>(pGameObject));
            }
            pObjectLayer->getGameObjects()->push_back(std::unique_ptr<GameObject>(pGameObject));
        }
    }
    pLayers->push_back(pObjectLayer);
}

void LevelParser::parseTileLayer(XMLElement* pTileElement, std::vector<Layer*> *pLayers, 
        const std::vector<Tileset>* pTilesets, std::vector<TileLayer*> *pCollisionLayers)
{
    TileLayer* pTileLayer = new TileLayer(m_tileSize, *pTilesets);
    bool collidable = false;
    // tile data
    std::vector<std::vector<int>> data;
    std::string decodedIDs;
    XMLElement* pDataNode;
    for(XMLElement* e = pTileElement->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
    {
        if(e->Value() == std::string("properties"))
        {
            for(XMLElement* property = e->FirstChildElement(); property != NULL; property = property->NextSiblingElement())
            {
                if(property->Value() == std::string("property")) {
                    if(property->Attribute("name") == std::string("collidable")) {
                        collidable = true;
                    }
                }
            }
        }
        if(e->Value() == std::string("data")) {
            pDataNode = e;
        }
    }

    for(XMLNode* e = pDataNode->FirstChild(); e != NULL; e = e->NextSibling())
    {
        XMLText* text = e->ToText();
        std::string t = text->Value();
        decodedIDs = base64_decode(t);
    }

    // uncompress zlib compression
    uLongf sizeofids = m_width * m_height * sizeof(int);
    std::vector<int> ids(m_width * m_height);
    uncompress((Bytef*)&ids[0], &sizeofids,(const Bytef*)decodedIDs.c_str(), decodedIDs.size());

    std::vector<int> layerRow(m_width);
    for(int j = 0; j < m_height; j++) {
        data.push_back(layerRow);
    }

    for(int rows = 0; rows < m_height; rows++) {
        for(int cols = 0; cols < m_width; cols++) {
            data[rows][cols] = ids[rows * m_width + cols];
        }
    }

    pTileLayer->setTileIDs(data);
    pTileLayer->setMapWidth(m_width);

    if(collidable) {
        pCollisionLayers->push_back(pTileLayer);
    }
    pLayers->push_back(pTileLayer);
}

