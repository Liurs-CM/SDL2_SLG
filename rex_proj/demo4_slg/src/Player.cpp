#include "Player.h"
#include "Camera.h"
#include "InputHandler.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "GameLib.h"
#include "Game.h"

Player::Player() : SDLGameObject() {}

const char* Player::dirNames[4] = {"Up", "Down", "Left", "Right"};

void Player::load(std::unique_ptr<LoaderParams> const &pParams)
{
    SDLGameObject::load(std::move(pParams));
    m_position = Vector2D(GRID_X + CELL_SIZE * 5, GRID_Y + CELL_SIZE * 5);
    m_delayFrame = FPS / m_animSpeed;
}

void Player::draw()
{
    //SDLGameObject::draw();
    //TheTextureManager::Instance()->drawPrintf(10, 220, COLOR_GREEN, "hi,move[%d],Dire[%s],Frame[%d/%d]%d,dlyFrame[%d/%d]%d", moving, dirNames[m_currentRow-1], m_currentFrame, m_numFrames, m_animSpeed, m_animTimer, m_delayFrame, m_currentAnim.globalFrame_);
    TheTextureManager::Instance()->drawPrintf(10, 220, COLOR_GREEN, "hi,move[%d],Dir[%s],Frame[%d/%d],(%.0f,%.0f)", moving, dirNames[m_currentRow-1], m_currentFrame, m_numFrames, m_position.getX(), m_position.getY());
    TextureManager::Instance()->drawFrame(m_textureID,  at_positionScreen - Vector2D(5,5), m_width, m_height, m_currentRow, m_currentFrame);
    // m_position delay frame
    if(moving){
        TextureManager::Instance()->drawFrame(m_textureID, m_positionScreen - Vector2D(5,5), m_width, m_height, m_currentRow, 0, 32);
    }
}

void Player::update()
{
    handleInput();
    //SDLGameObject::update();
    handleAnimation();
    TheCamera::Instance()->follow(at_position);
    m_positionScreen = m_position - TheCamera::Instance()->getPosition();
    at_positionScreen = at_position - TheCamera::Instance()->getPosition();
}

void Player::clean() { }

void Player::handleInput()
{
    if (TheInputHandler::Instance()->getMouseButtonState(LEFT))
    {
        Vector2D delt_pos = *(TheInputHandler::Instance()->getMousePosition()) - m_position;
        int x = int(delt_pos.getX() / CELL_SIZE);
        int y = int(delt_pos.getY() / CELL_SIZE);
        to_position = m_position + Vector2D(x, y) * CELL_SIZE;
        //to_position = m_position + Vector2D(0,-1) * CELL_SIZE;
        m_currentDirection = Direction::DOWN;
        moving = true;
    }
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
    if(TheInputHandler::Instance()->isKeyPressed(SDL_SCANCODE_SPACE))
    {
        TheSoundManager::Instance()->playSound("shoot", 0);
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
            at_position += dist.get_normalize() * shift_speed;
        }
    }
}

void Player::handleAnimation()
{
    m_animTimer++;
    if(m_animTimer >= m_delayFrame)
    {
        m_animTimer -= m_delayFrame;
        if (moving) {
            m_currentFrame = (m_currentFrame >= 2) ? -1 : m_currentFrame;
            m_currentFrame += 1;
            m_currentFrame = (m_currentFrame == -1) ? 2 : m_currentFrame;
        } else {
            m_currentFrame = (m_currentFrame == 0) ? 3 : 0; // standing frame
        }
    }
}

