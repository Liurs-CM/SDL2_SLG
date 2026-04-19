#include "Level.h"
#include "TextureManager.h"
#include "Layer.h"
#include "TileLayer.h"
#include "Game.h"
#include <math.h>
#include <iostream>

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
    for(auto* obj : m_layers)
    {
        obj->render();
    }
}

void Level::update()
{
    for(auto* obj : m_layers)
    {
        obj->update(this);
    }
}
