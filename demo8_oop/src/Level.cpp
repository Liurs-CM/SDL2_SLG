#include "Level.h"
#include "Layer.h"
#include <math.h>

Level::Level() { }

Level::~Level()
{
    for(auto* obj : m_layers)
    {
        delete obj;
    }
    m_layers.clear();
}

void Level::render()
{
    //GameLib::forEachCall(m_layers, &Layer::render);
    for(auto* obj : m_layers)
    {
        obj->render();
    }
}

void Level::update()
{
    if(m_pPlayer->handleInput()){
        for(auto* obj : m_layers)
        {
            obj->update(this);
        }
    }
}
