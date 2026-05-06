#include "LevelParser.h"
#include "TextureManager.h"
#include "ObjectLayer.h"
#include "TileLayer.h"
#include "GameObjectFactory.h"
#include "Level.h"
#include "lib/base64.h"
#include "lib/zlib.h"
#include <string>

// 辅助函数：检查元素名称是否匹配
inline bool isElementNamed(const tinyxml2::XMLElement* element, const char* name) {
    return element && element->Value() && std::strcmp(element->Value(), name) == 0;
}

// 辅助函数：安全获取整数属性
inline int getAttributeInt(const tinyxml2::XMLElement* element, const char* name, int defaultValue = 0) {
    int value = defaultValue;
    element->QueryIntAttribute(name, &value);
    return value;
}

// 辅助函数：安全获取字符串属性
inline std::string getAttributeString(const tinyxml2::XMLElement* element, const char* name, const char* defaultValue = "") {
    const char* value = element->Attribute(name);
    return value ? std::string(value) : std::string(defaultValue);
}

Level* LevelParser::parseLevel(const char *levelFile)
{
    // create a tinyXML document and load the map xml
    XMLDocument levelDocument;
    if(levelDocument.LoadFile(levelFile) != XML_SUCCESS) {
        std::cerr << "Failed to load level file: " << levelDocument.ErrorStr() << "\n";
        return nullptr;
    }

    // get the root node and display some values
    XMLElement* pRoot = levelDocument.RootElement();
    std::cout << "Loading level:\n"
        << "Version: " << getAttributeString(pRoot, "version") << "\n"
        << "Width:" << getAttributeString(pRoot, "width") 
        << " - Height:" << getAttributeString(pRoot, "height") << "\n"
        << "Tile Width:" << getAttributeString(pRoot, "tilewidth") 
        << " - Tile Height:" << getAttributeString(pRoot, "tileheight") << "\n";
    m_tileSize = getAttributeInt(pRoot, "tilewidth");
    m_width = getAttributeInt(pRoot, "width");
    m_height = getAttributeInt(pRoot, "height");

    // create the level object
    Level* pLevel = new Level();
    // we must parse the textures needed for this level, which have been added to properties
    for(XMLElement* e = pRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
    {
        //we know that properties is the first child of the root
        if(isElementNamed(e, "properties")) {
            for(auto prop = e->FirstChildElement(); prop; prop = prop->NextSiblingElement()) {
                if(isElementNamed(prop, "property")) { parseTextures(prop); }
            }
        }
        // we must now parse the tilesets
        else if(isElementNamed(e, "tileset")) {
            parseTilesets(e, pLevel->getTilesets());
        }
        // parse any object layers
        else if(isElementNamed(e, "objectgroup") || isElementNamed(e, "layer")) {
            if(isElementNamed(e->FirstChildElement(), "object")) {
                parseObjectLayer(e, pLevel->getLayers(), pLevel);
            }
            else if(isElementNamed(e->FirstChildElement(), "data") ||
                    (e->FirstChildElement()->NextSiblingElement() != 0 && 
                     e->FirstChildElement()->NextSiblingElement()->Value() == std::string("data"))) {
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

void LevelParser::parseObjectLayer(tinyxml2::XMLElement* pObjectElement, std::vector<Layer*>* pLayers, Level* pLevel) 
{
    ObjectLayer* pObjectLayer = new ObjectLayer();
    // 提前定义默认值
    int x = 0, y = 0, width = 0, height = 0, numFrames = 1, callbackID = 0, animSpeed = 4;
    std::string type, textureID;
    // 遍历所有对象元素
    for (auto e = pObjectElement->FirstChildElement(); e; e = e->NextSiblingElement()) 
    {
        if (!isElementNamed(e, "object")) continue;
        // 重置默认值
        x = getAttributeInt(e, "x");
        y = getAttributeInt(e, "y");
        type = getAttributeString(e, "type");
        width = height = 0;
        numFrames = 1;
        callbackID = 0;
        animSpeed = 4;
        textureID.clear();
        // 创建游戏对象
        GameObject* pGameObject = TheGameObjectFactory::Instance()->create(type);
        if (!pGameObject) {
            std::cerr << "Failed to create game object of type: " << type << "\n";
            continue;
        }
        // 解析属性 - 提取为单独函数以减少嵌套
        parseObjectProperties(e, width, height, numFrames, textureID, callbackID, animSpeed);
        // 加载游戏对象
        pGameObject->load(std::make_unique<LoaderParams>(x, y, width, height, textureID, numFrames, callbackID, animSpeed));
        // 设置玩家引用（如果适用）
        if (type == "Player") {
            pLevel->setPlayer(dynamic_cast<Player*>(pGameObject));
        }
        pObjectLayer->getGameObjects()->push_back(std::unique_ptr<GameObject>(pGameObject));
    }
    pLayers->push_back(pObjectLayer);
}

void LevelParser::parseObjectProperties(tinyxml2::XMLElement* objectElement, int& width, int& height, int& numFrames, std::string& textureID, int& callbackID, int& animSpeed) 
{
    // 属性名到变量的映射（仅适用于整型）
    std::unordered_map<std::string, int*> intProps = {
        {"numFrames", &numFrames},
        {"textureHeight", &height},
        {"textureWidth", &width},
        {"callbackID", &callbackID},
        {"animSpeed", &animSpeed}
    };
    for (auto properties = objectElement->FirstChildElement(); properties; properties = properties->NextSiblingElement()) 
    {
        if (!isElementNamed(properties, "properties")) continue;
        for (auto property = properties->FirstChildElement(); property; property = property->NextSiblingElement()) 
        {
            if (!isElementNamed(property, "property")) continue;

            std::string propName = getAttributeString(property, "name");
            if (propName == "textureID") {
                textureID = getAttributeString(property, "value");
            } else if (auto it = intProps.find(propName); it != intProps.end()) {
                *(it->second) = getAttributeInt(property, "value");
            }
        }
    }
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
        t.erase(std::remove_if(t.begin(), t.end(), ::isspace), t.end());
        decodedIDs += base64_decode(t);
    }

    // uncompress zlib compression
    std::vector<uint32_t> ids(m_width * m_height);
    uLongf sizeofids = ids.size() * sizeof(uint32_t);
    int uncompress_result = uncompress(
            (Bytef*)ids.data(), 
            &sizeofids,
            (const Bytef*)decodedIDs.data(), 
            decodedIDs.size());
    if(uncompress_result != Z_OK) {
        std::cerr << "zlib uncompress failed! Error code: " << uncompress_result << "\n";
        // 可能原因：数据不是 zlib 压缩、损坏、buffer 太小等
    }
    if (sizeofids != ids.size() * sizeof(uint32_t)) {
        std::cerr << "Decompressed size mismatch!\n";
    }

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
    pTileLayer->setMapHeight(m_height);
    pTileLayer->setMapWidth(m_width);
    //std::cout << "Map hight-width:" << m_width << "," << m_height << "!\n";

    if(collidable) {
        pCollisionLayers->push_back(pTileLayer);
    }
    pLayers->push_back(pTileLayer);
}

