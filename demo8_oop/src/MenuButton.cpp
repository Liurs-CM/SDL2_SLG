#include "MenuButton.h"
#include "InputHandler.h"
#include "TextureManager.h"

MenuButton::MenuButton() : SDLGameObject(), m_callback(0), m_bReleased(true) { }

void MenuButton::load(std::unique_ptr<LoaderParams> const &pParams)
{
    SDLGameObject::load(std::move(pParams));
    m_callbackID = pParams->getCallbackID();
    m_animClip.m_currentFrame = MOUSE_OUT;
}

void MenuButton::draw()
{
    //SDLGameObject::draw();
    TextureManager::Instance()->drawFrame(m_textureID, m_position, m_width, m_height, m_animClip.m_currentRow, m_animClip.m_currentFrame);
}

void MenuButton::update()
{
    vec* pMousePos = TheInputHandler::Instance()->getMousePosition();
    if(pMousePos->x < (m_position.x + m_width) && 
            pMousePos->x > m_position.x && 
            pMousePos->y < (m_position.y + m_height) && 
            pMousePos->y > m_position.y)
    {
        if(TheInputHandler::Instance()->getMouseButtonState(LEFT) && m_bReleased)
        {
            m_animClip.m_currentFrame = CLICKED;
            if(m_callback != 0) {
                m_callback();
            }
            m_bReleased = false;
        }
        else if(!TheInputHandler::Instance()->getMouseButtonState(LEFT))
        {
            m_bReleased = true;
            m_animClip.m_currentFrame = MOUSE_OVER;
        }
    }
    else
    {
        m_animClip.m_currentFrame = MOUSE_OUT;
    }
}

void MenuButton::clean() { }
