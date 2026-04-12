#ifndef __StateParser__
#define __StateParser__
#include <iostream>
#include <vector>
#include "tinyxml2.h"
using namespace tinyxml2;

class GameObject;

class StateParser
{
    public:
        bool parseState(const char* stateFile, std::string stateID, std::vector<GameObject*> *pObjects, std::vector<std::string> *pTextureIDs);
    private:
        void parseObjects(XMLElement* pStateRoot, std::vector<GameObject*> *pObjects);
        void parseTextures(XMLElement* psStateRoot, std::vector<std::string> *pTextureIDs);
};

#endif /* defined(__StateParser__) */

