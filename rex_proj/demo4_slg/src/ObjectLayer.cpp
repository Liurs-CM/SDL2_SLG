#include "ObjectLayer.h"
#include "GameLib.h"

ObjectLayer::~ObjectLayer()
{
    m_gameObjects.clear();
}

void ObjectLayer::update(Level* pLevel)
{
    GameLib::forEachCall(m_gameObjects, &GameObject::update);
}

void ObjectLayer::render()
{
    GameLib::forEachCall(m_gameObjects, &GameObject::draw);
}
