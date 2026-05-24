#include "Cat.h"

void Cat::load(std::unique_ptr<LoaderParams> const &pParams)
{
    SDLGameObject::load(std::move(pParams));
    //m_velocity.x = 0.2;
    //std::cout << "Cat size: " << m_height << "," << m_width << "\n";
    //std::cout << "Cat speed: " << m_velocity << "\n";
}

void Cat::update()
{
    switch(m_animClip.m_currentAction){
        case UnitAction::IDLE:
            if(AnimatedGraphic::m_currentPhase == Phase::EXECUTION){
                set2Pos(vec{1,0}, false);
                m_animClip.m_currentAction = UnitAction::MOVING;
            }
            break;
        case UnitAction::MOVING:
            if(!move()){
                m_animClip.m_currentAction = UnitAction::IDLE;
            }
            break;
        case UnitAction::ATTACKING:
            m_animClip.m_currentAction = UnitAction::IDLE;
            break;
        case UnitAction::DEAD:
            m_animClip.m_currentAction = UnitAction::IDLE;
            break;
    }
    SDLGameObject::update();
}

void Cat::set2Pos(vec Pos, bool to) 
{
    to_position = vec(m_position) + Pos * CELL_SIZE * m_animClip.m_animSpeed;
    to_position.x = (to_position.x > (SCR_W - m_width) ) ?  (SCR_W - m_width) : 
        (to_position.x < 0) ? 0 : 
        to_position.x;
    if(to_position == vec(m_position)){
        setAction(UnitAction::IDLE);
    }
    else{
        vec delt_pos = vec2(to_position) - m_position;
        if(delt_pos.x != 0 && delt_pos.y != 0){
            delt_pos = {delt_pos.x / CELL_SIZE, delt_pos.y / CELL_SIZE};
            to_position = vec(m_position) + delt_pos * CELL_SIZE;
            m_velocity = delt_pos.normalize() * m_animClip.m_animSpeed ;
            //m_currentDirection = delt_pos.getVecDir();
            //m_animClip.m_currentRow = static_cast<int>(m_currentDirection);
            setAction(UnitAction::MOVING);
        }
    }
}

bool Cat::move() 
{
    vec2 dist = vec2(to_position) - m_position;
    if(dist.distance() < m_animClip.m_animSpeed) {
        m_position = to_position;
        setAction(UnitAction::IDLE);
        m_velocity = vec{0, 0};
        return false;
    }
    else {
        //m_position += m_velocity;
    }
    return true;
}
