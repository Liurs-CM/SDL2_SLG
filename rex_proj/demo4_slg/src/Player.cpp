#include "Player.h"
#include "Camera.h"
#include "InputHandler.h"
#include "TextureManager.h"
#include "BulletHandler.h"
#include "SoundManager.h"
#include "GameLib.h"
#include "Game.h"

Player::Player() : SDLGameObject() {}

const char* Player::dirNames[4] = {"Up", "Down", "Left", "Right"};

void Player::load(std::unique_ptr<LoaderParams> const &pParams)
{
    SDLGameObject::load(std::move(pParams));
    m_position = Vector2D(GRID_X + CELL_SIZE * 5, GRID_Y + CELL_SIZE * 5);
    at_position = m_position;
}

void Player::draw()
{
    //SDLGameObject::draw();
    //TextureManager::Instance()->drawFrame(m_textureID, (Uint32)m_position.getX(), (Uint32)m_position.getY(), m_width, m_height, m_currentRow, m_currentFrame);
    TheTextureManager::Instance()->drawPrintf(10, 220, COLOR_GREEN, "hi,move[%d],Dire[%s],Frame[%d/%d],FPS %d,period %d", moving, dirNames[m_currentRow-1], m_currentFrame, m_numFrames, m_animSpeed, m_currentAnim.getFrameTime());
    TextureManager::Instance()->drawFrame(m_textureID, (Uint32)at_position.getX() - 5, (Uint32)at_position.getY() - 5, m_width, m_height, m_currentRow, m_currentFrame);
    // m_position delay frame
    if(moving){
        TextureManager::Instance()->drawFrame(m_textureID, (Uint32)m_position.getX() - 5, (Uint32)m_position.getY() - 5, m_width, m_height, m_currentRow, m_currentFrame, 128);
    }
}

void Player::update()
{
    handleInput();
    //SDLGameObject::update();
    m_currentAnim.update();
    handleAnimation();
    TheCamera::Instance()->follow(m_position);
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
        TheBulletHandler::Instance()->addBullet(m_position.getX() + 24, m_position.getY() + 24, 32, 16, "bullet", 1, Vector2D(1,0));
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
    if (moving) {
        m_currentFrame = (m_currentFrame >= 2) ? -1 : m_currentFrame;
        m_currentFrame += 1;
        m_currentFrame = (m_currentFrame == -1) ? 2 : m_currentFrame;
    } else {
        m_currentFrame = (m_currentFrame == 0) ? 3 : 0; // standing frame
    }
}

// if the player is not invulnerable then set to dying and change values for death animation tile sheet
void Player::collision()
{
    if(!TheGame::Instance()->getLevelComplete())
    {
        m_textureID = "largeexplosion";
        m_currentFrame = 0;
        m_numFrames = 9;
        m_width = 60;
        m_height = 60;
        m_bDying = true;
    }
}


