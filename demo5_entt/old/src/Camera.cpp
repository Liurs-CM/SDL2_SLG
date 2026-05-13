#include "Camera.h"
#include "GameLib.h"
#include <iostream>

Camera* Camera::s_pInstance = 0;

Camera::Camera() :
    m_position(0,0),
    m_width(SCR_W),
    m_height(SCR_H)
{ }

void Camera::follow(Vector2D target)
{
    m_position = Vector2D(target.getX() - m_width / 2, target.getY() - m_height / 2);
    m_position.setX(std::max(0.0f, std::min(m_position.getX(), float(MAP_W - m_width))));
    m_position.setY(std::max(0.0f, std::min(m_position.getY(), float(MAP_H - m_height))));
}

