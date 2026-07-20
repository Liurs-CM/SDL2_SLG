#pragma once
#include <SDL2/SDL_render.h>
#include <map>
#include <string>

class TextureManager 
{
    public:
        explicit TextureManager(SDL_Renderer* renderer) : m_renderer(renderer) {}
        bool load(std::string fileName, std::string id);
        void injectTexture(const std::string& id, SDL_Texture* tex) {
            if (tex && m_textureMap.find(id) == m_textureMap.end())
                m_textureMap[id] = tex;
        }
        void clearTextureMap();
        void clearFromTextureMap(std::string id);
        std::map<std::string, SDL_Texture*> getTextureMap() { return m_textureMap; }
        SDL_Texture* getTexture(std::string id) { 
            auto it = m_textureMap.find(id);
            return (it != m_textureMap.end()) ? it->second : nullptr;
        }
        static TextureManager* Instance()
        {
            if(s_pInstance == 0)
            {
                s_pInstance = new TextureManager();
                return s_pInstance;
            }
            return s_pInstance;
        }
    private:
        TextureManager() {}
        std::map<std::string, SDL_Texture*> m_textureMap;
        SDL_Renderer* m_renderer;
        static TextureManager* s_pInstance;
};

typedef TextureManager TheTextureManager;
