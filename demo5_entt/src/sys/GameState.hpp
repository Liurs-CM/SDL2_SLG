#pragma once
#include <entt/entity/registry.hpp>
#include <SDL.h>
#include <vector>
#include <string>

class GameState 
{
    public:
        virtual ~GameState() = default;
        virtual void enter(entt::registry& reg) = 0;   // 传入 registry 初始化实体
        virtual void exit(entt::registry& reg) = 0;    // 清理实体
        virtual void handleEvents(SDL_Event& e, entt::registry& reg) = 0;
        virtual void update(float dt, entt::registry& reg) = 0;
        virtual void render(SDL_Renderer* renderer, entt::registry& reg) = 0;
        virtual std::string getStateID() const = 0;
    protected:	
        GameState() : m_loadingComplete(false), m_exiting(false) {}
        bool m_loadingComplete;
        bool m_exiting;
        std::vector<std::string> m_textureIDList;
};
