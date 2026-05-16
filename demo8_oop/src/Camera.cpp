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

    //// 计算理想位置（让目标居中）
    //Vector2D to_position = Vector2D(target.getX() - m_width / 2, target.getY() - m_height / 2);
    //// 应用边界限制
    //to_position.setX(std::max(0, std::min(int(to_position.getX()), MAP_W - m_width)));
    //to_position.setY(std::max(0, std::min(int(to_position.getY()), MAP_H - m_height)));
    //// 可以在这里添加平滑移动、震动等效果
    //if(m_position != to_position){
    //    Vector2D dist = to_position - m_position;
    //    if(dist.length() < 1.0f) {
    //        m_position = to_position;
    //    }
    //    else {
    //        m_position += dist.get_normalize() * shift_speed;
    //    }
    //}
}

