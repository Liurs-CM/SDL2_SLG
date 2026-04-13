#include "Player.h"
#include "InputHandler.h"
#include "TextureManager.h"

Player::Player() : SDLGameObject() {}
const char* Player::dirNames[4] = {"Up", "Down", "Left", "Right"};

void Player::draw()
{
    //SDLGameObject::draw();
    TextureManager::Instance()->drawFrame(m_textureID, (Uint32)m_position.getX(), (Uint32)m_position.getY(), m_width, m_height, m_currentRow, m_currentFrame);
    TheTextureManager::Instance()->drawPrintf(10, 220, COLOR_GREEN, "hi, test printf move[%d], Direction: %s, Frame[%d]", moving, dirNames[m_currentRow-1], m_currentFrame);
}

void Player::update()
{
	handleInput();
    //SDLGameObject::update();
    animTimer = (animTimer >= 250.0f) ? 0 : (animTimer + DELAY_TIME);
    if(animTimer == 0){
        if (moving) {
            m_currentFrame = (m_currentFrame >= 2) ? -1 : m_currentFrame;
            m_currentFrame += 1;
            m_currentFrame = (m_currentFrame == -1) ? 2 : m_currentFrame;
        } else {
            m_currentFrame = (m_currentFrame == 0) ? 3 : 0; // standing frame
        }
    }
}

void Player::clean() { }

void Player::handleInput()
{
    //Vector2D* target = TheInputHandler::Instance()->getMousePosition();
    //m_velocity = *target - m_m_positionition;
    //m_velocity /= 50;
    moving = false;
    if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_W))
    {
        m_position += Vector2D(0,-1) * speed;
        m_currentDirection = Direction::UP;
        moving = true;
        //std::cout << m_position << "move up\n";
    }
    if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_S))
    {
        m_position += Vector2D(0,1) * speed;
        m_currentDirection = Direction::DOWN;
        moving = true;
        //std::cout << m_position << "move down\n";
    }
    if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_A))
    {
        m_position += Vector2D(-1,0) * speed;
        m_currentDirection = Direction::LEFT;
        moving = true;
        //std::cout << m_position << "move left\n";
    }
    if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_D))
    {
        m_position += Vector2D(1,0) * speed;
        m_currentDirection = Direction::RIGHT;
        moving = true;
        //std::cout << m_position << "move right\n";
    }
    m_currentRow = static_cast<int>(m_currentDirection);
}

void Player::load(const LoaderParams *pParams)
{
    SDLGameObject::load(pParams);
}
