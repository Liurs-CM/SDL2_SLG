#include "Player.h"
#include "InputHandler.h"

Player::Player() : SDLGameObject() {}

void Player::draw()
{
    SDLGameObject::draw();
}

void Player::update()
{
	handleInput();
    SDLGameObject::update();
}

void Player::clean() { }

void Player::handleInput()
{
    //Vector2D* target = TheInputHandler::Instance()->getMousePosition();
    //m_velocity = *target - m_m_positionition;
    //m_velocity /= 50;
    if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_W))
    {
        m_position += Vector2D(0,-1);
        //std::cout << m_position << "move up\n";
    }
    if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_S))
    {
        m_position += Vector2D(0,1);
        //std::cout << m_position << "move down\n";
    }
    if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_A))
    {
        m_position += Vector2D(-1,0);
        //std::cout << m_position << "move left\n";
    }
    if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_D))
    {
        m_position += Vector2D(1,0);
        //std::cout << m_position << "move right\n";
    }
}

void Player::load(const LoaderParams *pParams)
{
    SDLGameObject::load(pParams);
}
