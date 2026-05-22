#pragma once
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_scancode.h"
#include "SDL2/SDL_joystick.h"
#include "Vector2D.h"
#include <array>
#include <vector>
#include <unordered_set>
#include <unordered_map>

enum mouse_buttons
{
	LEFT = 0,
	MIDDLE = 1,
	RIGHT = 2
};

struct DirVec {
    Direction dir;
    vec2d<> vec;
};

// 按键映射：SDL_Keycode -> 方向矢量
static const std::unordered_map<SDL_Scancode, DirVec> key2DirVec = {
    { SDL_SCANCODE_W, {Direction::UP       , vec2d<>::up }}, // 上
    { SDL_SCANCODE_S, {Direction::DOWN     , vec2d<>::down}}, // 下
    { SDL_SCANCODE_A, {Direction::LEFT     , vec2d<>::left}}, // 左
    { SDL_SCANCODE_D, {Direction::RIGHT    , vec2d<>::right}}, // 右
    { SDL_SCANCODE_H, {Direction::LEFT     , vec2d<>::left}}, // 左
    { SDL_SCANCODE_J, {Direction::DOWN     , vec2d<>::down}}, // 下
    { SDL_SCANCODE_K, {Direction::UP       , vec2d<>::up}}, // 上
    { SDL_SCANCODE_L, {Direction::RIGHT    , vec2d<>::right}}, // 右
    { SDL_SCANCODE_Y, {Direction::UP  , vec2d<>::leftUp}}, // 左上
    { SDL_SCANCODE_U, {Direction::UP , vec2d<>::rightUp}}, // 右上
    { SDL_SCANCODE_B, {Direction::DOWN, vec2d<>::leftDown}}, // 左下
    { SDL_SCANCODE_N, {Direction::DOWN, vec2d<>::rightDown}}  // 右下
};


class InputHandler
{
    public:
        static InputHandler* Instance()
        {
            if(s_pInstance == nullptr)
            {
                s_pInstance = new InputHandler();
            }
            return s_pInstance;
        }
        void update();
        void clean();
        void reset_mouseButtonState();
        void initialiseJoysticks();
        bool joysticksInitialised() { return m_bJoysticksInitialised; }
        vec2d<>* getMousePosition()
        {
            return m_mousePosition;
        }
        bool isKeyDown(SDL_Scancode key) const;
        bool isKeyPressed(SDL_Scancode key) const;
        bool getMouseButtonState(int buttonNumber)
        {
            return m_mouseButtonStates[buttonNumber];
        }	
        bool getButtonState(int joy, int buttonNumber)
        {
            return m_buttonStates[joy][buttonNumber];
        }

    private:
        InputHandler();
        ~InputHandler() { delete m_mousePosition; }
        static InputHandler* s_pInstance;
        std::vector<SDL_Joystick*> m_joysticks;
        bool m_bJoysticksInitialised;
        std::vector<std::pair<vec2d<>*, vec2d<>*>> m_joystickValues;
        std::vector<std::vector<bool>> m_buttonStates;
        std::array<bool, 3> m_mouseButtonStates{};
        vec2d<>* m_mousePosition;
        const Uint8* m_keystates;
        std::unordered_set<SDL_Scancode> m_keysPressedThisFrame; // 本帧新按下的键（非 repeat）
        int xvalue(int joy, int stick);
        int yvalue(int joy, int stick);

        const int m_joystickDeadZone = 10000;

        // handle keyboard events
        void onKeyDown();
        void onKeyUp();

        // handle mouse events
        void onMouseMove(SDL_Event& event);
        void onMouseButtonDown(SDL_Event& event);
        void onMouseButtonUp(SDL_Event& event);

        // handle joysticks events
        void onJoystickAxisMove(SDL_Event& event);
        void onJoystickButtonDown(SDL_Event& event);
        void onJoystickButtonUp(SDL_Event& event);
};

typedef InputHandler TheInputHandler;

