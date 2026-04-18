//
//  Map.cpp
//  SDL Game Programming Book
//
//  Created by shaun mitchell on 09/03/2013.
//  Copyright (c) 2013 shaun mitchell. All rights reserved.
//

#include "Level.h"
#include "TextureManager.h"
#include "Game.h"
#include "Layer.h"
#include "TileLayer.h"
#include <math.h>
#include <iostream>

Level::Level()
{
}

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
