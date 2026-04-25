#ifndef __ObjectLayer__
#define __ObjectLayer__
#include <memory>
#include <vector>
#include "GameObject.h"
#include "Layer.h"

class ObjectLayer : public Layer
{
    public:
        virtual ~ObjectLayer();
        virtual void update(Level* pLevel);
        virtual void render();
        std::vector<std::unique_ptr<GameObject>>* getGameObjects() { 
            return &m_gameObjects; }
    private:
        std::vector<std::unique_ptr<GameObject>> m_gameObjects;
};

#endif // __ObjectLayer__
