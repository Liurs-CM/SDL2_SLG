#ifndef __GameObject__
#define __GameObject__
#include "LoaderParams.h"
#include "Vector2D.h"
#include <string>
#include <memory>

struct Movement
{
    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;
    Movement(Vector2D pos) : position(pos), velocity(0,0), acceleration(0,0) {}
};



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

#endif // __GameObject__
