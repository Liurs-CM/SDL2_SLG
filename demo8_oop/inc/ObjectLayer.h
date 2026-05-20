#pragma once
#include "Layer.h"
#include "GameObject.h"
#include <memory>
#include <vector>

class ObjectLayer : public Layer
{
    public:
        virtual ~ObjectLayer();
        virtual void update(Level* pLevel);
        virtual void render();
        std::vector<std::unique_ptr<GameObject>>* getGameObjects() { return &m_gameObjects; }
    private:
        std::vector<std::unique_ptr<GameObject>> m_gameObjects;
};

