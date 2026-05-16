#include "StateParser.h"
#include "GameObjectFactory.h"
#include "Game.h"
#include <string>
#include <iostream>

bool StateParser::parseState(const char* stateFile, std::string stateID, std::vector<std::unique_ptr<GameObject>> *pObjects, std::vector<std::string> *pTextureIDs)
{
    // create the XML document
    XMLDocument xmlDoc;
    // load the state file
    if(xmlDoc.LoadFile(stateFile) != XML_SUCCESS) {
        std::cerr << xmlDoc.ErrorStr() << "\n";
        return false;
    }
    // get the root element
    XMLElement* pRoot = xmlDoc.RootElement(); // <STATES>

    // pre declare the states root node
    XMLElement* pStateRoot = 0;
    // get this states root node and assign it to pStateRoot
    for(XMLElement* e = pRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
        if(e->Value() == stateID) {
            pStateRoot = e;
        }
    }

    // pre declare the texture root
    XMLElement* pTextureRoot = 0;
    // get the root of the texture elements
    for(XMLElement* e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
        if(e->Value() == std::string("TEXTURES")) {
            pTextureRoot = e;
        }
    }
    // now parse the textures
    parseTextures(pTextureRoot, pTextureIDs);

    // pre declare the object root node
    XMLElement* pObjectRoot = 0;
    // get the root node and assign it to pObjectRoot
    for(XMLElement* e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
        if(e->Value() == std::string("OBJECTS")) {
            pObjectRoot = e;
        }
    }
    // now parse the objects
    parseObjects(pObjectRoot, pObjects);

    return true;
}

void StateParser::parseTextures(XMLElement* pStateRoot, std::vector<std::string> *pTextureIDs)
{
    for(XMLElement* e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
    {
        std::string filenameAttribute = e->Attribute("filename");
        std::string idAttribute = e->Attribute("ID");
        pTextureIDs->push_back(idAttribute); // push into list
        TheTextureManager::Instance()->load(filenameAttribute, idAttribute);
    }
}

void StateParser::parseObjects(XMLElement *pStateRoot, std::vector<std::unique_ptr<GameObject>> *pObjects)
{
    for(XMLElement* e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
    {
        int x, y, width, height, numFrames, callbackID;
        float animSpeed;
        std::string textureID;

        e->QueryIntAttribute("x", &x);
        e->QueryIntAttribute("y", &y);
        e->QueryIntAttribute("width", &width);
        e->QueryIntAttribute("height", &height);
        e->QueryIntAttribute("numFrames", &numFrames);
        e->QueryIntAttribute("callbackID", &callbackID);
        e->QueryFloatAttribute("animSpeed", &animSpeed);
        textureID = e->Attribute("textureID");

        GameObject* pGameObject = TheGameObjectFactory::Instance()->create(e->Attribute("type"));
        pGameObject->load(std::unique_ptr<LoaderParams>(new LoaderParams(x, y, width, height, textureID, numFrames, callbackID, animSpeed)));
        pObjects->push_back(std::unique_ptr<GameObject>(pGameObject));
    }
}
