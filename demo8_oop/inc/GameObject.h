#pragma once
#include "LoaderParams.h"
#include <string>
#include <memory>

class GameObject
{
    public:
        virtual ~GameObject() = default;
        virtual void load(std::unique_ptr<LoaderParams> const &pParams)=0;
        virtual void draw()=0;
        virtual void update()=0;
        virtual void clean()=0;
        virtual std::string type() = 0;
    protected:
        GameObject() {}
        std::string m_textureID;
};

