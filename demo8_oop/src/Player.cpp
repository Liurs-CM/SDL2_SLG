#include "Player.h"
#include "Camera.h"
#include "InputHandler.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "GameLib.h"

Player::Player() : SDLGameObject() {}

const char* Player::dirNames[4] = {"Up", "Down", "Left", "Right"};

void Player::load(std::unique_ptr<LoaderParams> const &pParams)
{
    SDLGameObject::load(std::move(pParams));
    m_position = vec(CELL_SIZE * 5, CELL_SIZE * 5);
    //m_position = Vector2D(GRID_X + CELL_SIZE * 5, GRID_Y + CELL_SIZE * 5);
    m_delayFrame = FPS / m_animSpeed;
}

void Player::draw()
{
    //SDLGameObject::draw();
    //TheTextureManager::Instance()->drawPrintf(10, 220, COLOR_GREEN, "hi,move[%d],Dire[%s],Frame[%d/%d]%d,dlyFrame[%d/%d]%d", moving, dirNames[m_currentRow-1], m_currentFrame, m_numFrames, m_animSpeed, m_animTimer, m_delayFrame, m_currentAnim.globalFrame_);
    TheTextureManager::Instance()->drawPrintf(10, 10, COLOR_GREEN, "hi,move[%d],\nDir[%s],\nFrame[%d/%d],\n(%.0f,%.0f)", moving, dirNames[m_currentRow-1], m_currentFrame, m_numFrames-1, m_position.x, m_position.y);
    TextureManager::Instance()->drawFrame(m_textureID,  at_positionScreen, m_width, m_height, m_currentRow, m_currentFrame);
    // m_position delay frame
    if(moving){
        TextureManager::Instance()->drawFrame(m_textureID, m_positionScreen, m_width, m_height, m_currentRow, 0, 32);
    }
    TextureManager::Instance()->drawHealthBar(at_positionScreen + vec(4,  -1), CELL_SIZE - 8, 4, 5 * m_currentAnim.globalFrame_, 1000);
}

void Player::update()
{
    //handleInput();
    //SDLGameObject::update();
    handleAnimation();
    TheCamera::Instance()->follow(at_position);
    m_positionScreen = m_position - TheCamera::Instance()->getPosition();
    at_positionScreen = vec2(at_position) - TheCamera::Instance()->getPosition();
}

void Player::clean() { }

bool Player::handleInput()
{
    if (TheInputHandler::Instance()->getMouseButtonState(LEFT))
    {
        vec2 delt_pos = vec2(*(TheInputHandler::Instance()->getMousePosition())) - m_position + TheCamera::Instance()->getPosition();
        int x = delt_pos.x / CELL_SIZE;
        int y = delt_pos.y / CELL_SIZE;
        to_position = m_position + vec2(x, y) * CELL_SIZE;
        //to_position = m_position + Vector2D(0,-1) * CELL_SIZE;
        m_currentDirection = Direction::DOWN;
        moving = true;
    }
    for (const auto& [key, dir_vec] : key2DirVec) 
    {
        if (TheInputHandler::Instance()->isKeyDown(key))
        {
            to_position = vec(m_position) + dir_vec.vec * CELL_SIZE * m_animSpeed;
            m_currentDirection = dir_vec.dir;
            moving = true;
            //std::cout << m_position << "move up\n";
        }
    }
    if(TheInputHandler::Instance()->isKeyPressed(SDL_SCANCODE_SPACE))
    {
        TheSoundManager::Instance()->playSound("shoot", 0);
    }
    m_currentRow = static_cast<int>(m_currentDirection);
    if(moving) {
        vec2 dist = vec2(to_position) - at_position;
        if(dist.distance() < 1.0f) {
            at_position = to_position;
            m_position = to_position;
            moving = false;
        }
        else {
            at_position += normalize(dist) * shift_speed;
        }
        //std::cout << m_position << "turn\n";
        return true;
    }
    return false;
}

void Player::handleAnimation()
{
    m_animTimer++;
    if(m_animTimer >= m_delayFrame)
    {
        m_animTimer -= m_delayFrame;
        if (moving) {
            m_currentFrame = (m_currentFrame >= 2) ? 0xff : m_currentFrame;
            m_currentFrame += 1;
            m_currentFrame = (m_currentFrame == 0xff) ? 2 : m_currentFrame;
        } else {
            m_currentFrame = (m_currentFrame == 0) ? 3 : 0; // standing frame
        }
    }
}

