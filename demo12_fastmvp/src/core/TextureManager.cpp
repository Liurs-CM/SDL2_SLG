#include "core/TextureManager.hpp"
#include "core/RenderContext.hpp"
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>
#include <string>
#include <iostream>

TheTextureManager* TextureManager::s_pInstance = 0;

bool TextureManager::load(std::string fileName,std::string id)
{ 
    m_renderer = RenderContext::get();
    SDL_Surface* pTempSurface = IMG_Load(fileName.c_str()); 
    if(pTempSurface == 0)
    {
        std::cout << "Error can't load img: " << fileName;
        return false;
    } 
    SDL_Texture* pTexture = SDL_CreateTextureFromSurface( m_renderer, pTempSurface);
    SDL_FreeSurface(pTempSurface);
    if( pTexture != 0 )
    {
        m_textureMap[id] = pTexture;
        return true;
    }
    return false;
}

void TextureManager::clearTextureMap()
{
    m_textureMap.clear();
}

void TextureManager::clearFromTextureMap(std::string id)
{
    m_textureMap.erase(id);
}

