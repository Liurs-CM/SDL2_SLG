#ifndef __Camera__
#define __Camera__
#include "Vector2D.h"
#include <cstdint>

class Camera
{
    public:
        void update(); // 更新相机逻辑
        Vector2D getPosition() const { return m_position; }
        void follow(Vector2D target); // 跟随某个目标点
        // ... 其他方法，如设置边界、震动效果等 ...
        static Camera* Instance()
        {
            if(s_pInstance == 0)
            {
                s_pInstance = new Camera();
                return s_pInstance;
            }
            return s_pInstance;
        }
    private:
        Camera();
        ~Camera();
        Vector2D m_position; // 相机的世界坐标（即视口左上角）
        uint16_t m_width, m_height; // 相机的视口尺寸
        static Camera* s_pInstance;
};

typedef Camera TheCamera;

#endif // __Camera__
