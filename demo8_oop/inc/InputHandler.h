#pragma once
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_scancode.h"
#include "SDL2/SDL_joystick.h"
#include "Vector2D.h"
#include <array>
#include <vector>
#include <utility>
#include <unordered_set>

enum mouse_buttons
{
	LEFT = 0,
	MIDDLE = 1,
	RIGHT = 2
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
        Vector2D* getMousePosition()
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
        std::vector<std::pair<Vector2D*, Vector2D*>> m_joystickValues;
        std::vector<std::vector<bool>> m_buttonStates;
        std::array<bool, 3> m_mouseButtonStates{};
        Vector2D* m_mousePosition;
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

