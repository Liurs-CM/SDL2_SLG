// core/TextureRegistry.cppm
export module core.TextureRegistry;
import <SDL2/SDL.h>;
import <SDL2/SDL_image.h>;
import <unordered_map>;
import <string>;
import <string_view>;
import <iostream>;

export struct TextureRegistry {
    std::unordered_map<std::string, SDL_Texture*> pool;

    bool load(SDL_Renderer* renderer, std::string_view path, std::string_view id) {
        if (pool.contains(std::string(id))) return true;
        SDL_Surface* surf = IMG_Load(path.data());
        if (!surf) {
            std::cerr << "[TextureRegistry] Failed to load: " << path << '\n';
            return false;
        }
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_FreeSurface(surf);
        if (tex) {
            pool.emplace(id, tex);
            return true;
        }
        return false;
    }

    SDL_Texture* get(std::string_view id) const {
        auto it = pool.find(std::string(id));
        return it != pool.end() ? it->second : nullptr;
    }

    void remove(std::string_view id) {
        auto it = pool.find(std::string(id));
        if (it != pool.end()) {
            SDL_DestroyTexture(it->second);
            pool.erase(it);
        }
    }

    void clear() {
        for (auto& [_, tex] : pool) SDL_DestroyTexture(tex);
        pool.clear();
    }

    ~TextureRegistry() { clear(); }
};
