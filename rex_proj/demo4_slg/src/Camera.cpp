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
    Vector2D to_position = Vector2D(target.getX() - m_width / 2, target.getY() - m_height / 2);
    // 应用边界限制
    to_position.setX(std::max(0, std::min(int(to_position.getX()), MAP_W - m_width)));
    to_position.setY(std::max(0, std::min(int(to_position.getY()), MAP_H - m_height)));
    // 可以在这里添加平滑移动、震动等效果
    if(m_position != to_position){
        Vector2D dist = to_position - at_position;
        if(dist.length() < 1.0f) {
            at_position = to_position;
            m_position = to_position;
        }
        else {
            at_position += dist.get_normalize() * shift_speed;
        }
    }
}

