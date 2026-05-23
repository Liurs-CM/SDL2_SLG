#include "Player.h"
#include "Camera.h"
#include "TextureManager.h"
#include "GameLib.h"

Player::Player() : SDLGameObject() {}

const char* Player::dirNames[4] = {"Up", "Down", "Left", "Right"};

void Player::load(std::unique_ptr<LoaderParams> const &pParams)
{
    SDLGameObject::load(std::move(pParams));
    m_position = vec(CELL_SIZE * 5, CELL_SIZE * 5);
    //m_position = Vector2D(GRID_X + CELL_SIZE * 5, GRID_Y + CELL_SIZE * 5);
}

void Player::draw()
{
    //SDLGameObject::draw();
    //TheTextureManager::Instance()->drawPrintf(10, 220, COLOR_GREEN, "hi,move[%d],Dire[%s],Frame[%d/%d]%d,dlyFrame[%d/%d]%d", moving, dirNames[m_currentRow-1], m_currentFrame, m_numFrames, m_animSpeed, m_animTimer, m_delayFrame, m_currentAnim.globalFrame_);
    TheTextureManager::Instance()->drawPrintf(10, 10, COLOR_GREEN, "hi,move[%d],\nDir[%s],\nFrame[%d/%d],\n(%.0f,%.0f)", moving, dirNames[m_animClip.m_currentRow-1], m_animClip.m_currentFrame, m_animClip.m_numFrames-1, m_position.x, m_position.y);
    TextureManager::Instance()->drawFrame(m_textureID,  at_positionScreen, m_width, m_height, m_animClip.m_currentRow, m_animClip.m_currentFrame);
    // m_position delay frame
    if(moving){
        TextureManager::Instance()->drawFrame(m_textureID, m_positionScreen, m_width, m_height, m_animClip.m_currentRow, 0, 32);
    }
    TextureManager::Instance()->drawHealthBar(at_positionScreen + vec(4,  -1), CELL_SIZE - 8, 4, 5 * m_animClip.m_currentAnim.globalFrame_, 1000);
}

void Player::update()
{
    //handleInput();
    //SDLGameObject::update();
    m_animClip.handleAnimation();
    TheCamera::Instance()->follow(at_position);
    m_positionScreen = m_position - TheCamera::Instance()->getPosition();
    at_positionScreen = vec2(at_position) - TheCamera::Instance()->getPosition();
}

void Player::clean() { }

void Player::set2Pos(vec Pos, bool to) 
{
    if(to){
        vec delt_pos = vec2(Pos) - m_position + TheCamera::Instance()->getPosition();
        delt_pos = {delt_pos.x / CELL_SIZE, delt_pos.y / CELL_SIZE};
        to_position = vec(m_position) + delt_pos * CELL_SIZE;
        m_currentDirection = delt_pos.getVecDir();
    }
    else{
        to_position = vec(m_position) + Pos * CELL_SIZE * m_animClip.m_animSpeed;
        m_currentDirection = Pos.getVecDir();
    }
    m_animClip.m_currentRow = static_cast<int>(m_currentDirection);
    setAction(UnitAction::MOVING);
}

void Player::move() 
{
    vec2 dist = vec2(to_position) - at_position;
    if(dist.distance() < m_animClip.m_animSpeed) {
        at_position = to_position;
        m_position = to_position;
        setAction(UnitAction::IDLE);
    }
    else {
        at_position += normalize(dist) * m_animClip.m_animSpeed * shift_speed;
    }

    //vec2 dist = vec2(to_position) - at_position;
    ////std::cout << "dist: " << dist << "\n";
    //if(dist.distance() < 1.0f) {
    //    at_position = to_position;
    //    m_position = to_position;
    //    moving = false;
    //    m_animClip.setAction(UnitAction::IDLE);
    //}
    //else {
    //    at_position += normalize(dist) * m_animClip.m_animSpeed;
    //    //std::cout << "at_position: " << at_position << "\n";
    //}
}
