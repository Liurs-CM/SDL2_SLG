#include "Player.h"
#include "InputHandler.h"
#include "TextureManager.h"
#include "GameLib.h"
#include "Game.h"

Player::Player() : SDLGameObject() {}
const char* Player::dirNames[4] = {"Up", "Down", "Left", "Right"};

void Player::draw()
{
    //SDLGameObject::draw();
    //TextureManager::Instance()->drawFrame(m_textureID, (Uint32)m_position.getX(), (Uint32)m_position.getY(), m_width, m_height, m_currentRow, m_currentFrame);
    TextureManager::Instance()->drawFrame(m_textureID, (Uint32)at_position.getX(), (Uint32)at_position.getY(), m_width, m_height, m_currentRow, m_currentFrame);
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
    if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_W))
    {
        to_position = m_position + Vector2D(0,-1) * CELL_SIZE;
        m_currentDirection = Direction::UP;
        moving = true;
        //std::cout << m_position << "move up\n";
    }
    if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_S))
    {
        to_position = m_position + Vector2D(0,1) * CELL_SIZE;
        m_currentDirection = Direction::DOWN;
        moving = true;
        //std::cout << m_position << "move down\n";
    }
    if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_A))
    {
        to_position = m_position + Vector2D(-1,0) * CELL_SIZE;
        m_currentDirection = Direction::LEFT;
        moving = true;
        //std::cout << m_position << "move left\n";
    }
    if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_D))
    {
        to_position = m_position + Vector2D(1,0) * CELL_SIZE;
        m_currentDirection = Direction::RIGHT;
        moving = true;
        //std::cout << m_position << "move right\n";
    }
    m_currentRow = static_cast<int>(m_currentDirection);
    if(moving) {
        Vector2D dist = to_position - at_position;
        if(dist.length() < 1.0f) {
            at_position = to_position;
            m_position = to_position;
            moving = false;
        }
        else {
            at_position += dist.get_normalize() * speed;
            //at_position += dist * speed;
            //at_position += dist * speed * TheGame::Instance()->getframeTime();
        }
    }
}

void Player::load(const LoaderParams *pParams)
{
    SDLGameObject::load(pParams);
    m_position = Vector2D(GRID_X + CELL_SIZE * 5, GRID_Y + CELL_SIZE * 5);
    at_position = m_position;
}
