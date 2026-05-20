#pragma once
#include "lib/tinyxml2.h"
#include <memory>
#include <vector>
using namespace tinyxml2;

class GameObject;

class StateParser
{
    public:
        bool parseState(const char* stateFile, std::string stateID, 
                std::vector<std::unique_ptr<GameObject>> *pObjects, std::vector<std::string> *pTextureIDs);
    private:
        void parseObjects(XMLElement* pStateRoot, std::vector<std::unique_ptr<GameObject>> *pObjects);
        void parseTextures(XMLElement* psStateRoot, std::vector<std::string> *pTextureIDs);
};

