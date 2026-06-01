#include "Cat.h"

void Cat::load(std::unique_ptr<LoaderParams> const &pParams)
{
    SDLGameObject::load(std::move(pParams));
    //m_velocity.x = 0.2;
    //std::cout << "Cat size: " << m_height << "," << m_width << "\n";
    //std::cout << "Cat speed: " << m_velocity << "\n";
}

int tmp_x = 1;
void Cat::update()
{
    switch(m_animClip.m_currentAction){
        case UnitAction::IDLE:
            //std::cout << "CatAct: movestart"  << movestart << "\n";
            if(AnimatedGraphic::m_currentPhase == Phase::EXECUTION){
                tmp_x = (to_position.x >= (SCR_W - m_width) ) ?  -1 : 
                    (to_position.x <= 0) ? 1 : tmp_x;
                set2Pos(vec{tmp_x,0}, false);
                m_animClip.m_currentAction = UnitAction::MOVING;
                //std::cout << "Ani: EXE" << "\n";
            }
            break;
        case UnitAction::MOVING:
            //std::cout << "CatAct: MOVING" << "\n";
            if(!move()){
                m_animClip.m_currentAction = UnitAction::IDLE;
                movestart = (AnimatedGraphic::m_currentPhase == Phase::INPUT);
                //std::cout << "Ani: MOV" << "\n";
            }
            break;
        case UnitAction::ATTACKING:
            //std::cout << "CatAct: ATTACKING" << "\n";
            m_animClip.m_currentAction = UnitAction::IDLE;
            break;
        case UnitAction::DEAD:
            //std::cout << "CatAct: DEAD" << "\n";
            m_animClip.m_currentAction = UnitAction::IDLE;
            break;
    }
    SDLGameObject::update();
}

void Cat::set2Pos(vec Pos, bool to) 
{
    to_position = vec(m_position) + Pos * (CELL_SIZE * m_animClip.m_animSpeed);
    to_position.x = (to_position.x > (SCR_W - m_width) ) ?  (SCR_W - m_width) : 
        (to_position.x < 0) ? 0 : 
        to_position.x;
    if(to_position == vec(m_position)){
        setAction(UnitAction::IDLE);
    }
    else{
        vec delt_pos = vec2(to_position) - m_position;
        if(delt_pos.x != 0 && delt_pos.y != 0){
            delt_pos /= CELL_SIZE;
            to_position = vec(m_position) + delt_pos * CELL_SIZE;
            m_velocity = normalize(delt_pos) * m_animClip.m_animSpeed ;
            //m_currentDirection = delt_pos.getVecDir();
            //m_animClip.m_currentRow = static_cast<int>(m_currentDirection);
            setAction(UnitAction::MOVING);
        }
    }
    //std::cout << "CatActSet: vel " << m_velocity << "\n";
}

bool Cat::move() 
{
    vec2 dist = vec2(to_position) - m_position;
    //std::cout << "CatAct: dist " << dist << " speed: " << m_animClip.m_animSpeed << "\n";
    if(dist.distance() < m_animClip.m_animSpeed) {
        m_position = to_position;
        setAction(UnitAction::IDLE);
        m_velocity = vec{0, 0};
        return false;
    }
    else {
        //std::cout << "CatAct: MOVING dist " << dist << " d: " << dist.distance() << " nor: " << normalize(dist) << "\n";
        m_velocity = normalize(dist) * m_animClip.m_animSpeed ;
        //m_position += m_velocity;
    }
    //std::cout << "CatAct: vel " << m_velocity << "\n";
    return true;
}
