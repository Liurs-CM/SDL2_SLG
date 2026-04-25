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
    // 计算理想位置（让目标居中）
    Vector2D idealPos = Vector2D(target.getX() - m_width / 2, target.getY() - m_height / 2);
    // 应用边界限制
    idealPos.setX(std::min(int(idealPos.getX()), MAP_W - m_width));
    idealPos.setY(std::min(int(idealPos.getY()), MAP_H - m_height));
    // 可以在这里添加平滑移动、震动等效果
    m_position = idealPos;
}

