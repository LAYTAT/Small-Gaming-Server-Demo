#include "TransformSystem.h"

TransformSystem::TransformSystem()
{

}

TransformSystem::~TransformSystem()
{

}

void TransformSystem::UpdatePosition(const PositionComponent & pos,const OrientComponent & ori,const SpeedComponent & spe)
{
    std::cout << "update position " << std::endl;



    return ;
}


bool TransformSystem::SetTrans(Entity* pE,int x,int y,int r,int s)
{
    if(!pE)
        return true;
    auto pCom_Base =pE->GetComponent(COMPONENTS_POSITION);
    if(!pCom_Base)
        return true;

    auto pCom_Pos =dynamic_cast<PositionComponent*>(pCom_Base);
    if(!pCom_Pos)
        return true;

    pCom_Pos->SetPos(x,y);

    pCom_Base =pE->GetComponent(COMPONENTS_ORIENT);
    if(!pCom_Base)
        return true;

    auto pCom_Ori =dynamic_cast<OrientComponent*>(pCom_Base);
    if(!pCom_Ori)
        return true;

    pCom_Ori->SetOri(r);

    pCom_Base =pE->GetComponent(COMPONENTS_SPEED);
    if(!pCom_Base)
        return true;

    auto pCom_Spd =dynamic_cast<SpeedComponent*>(pCom_Base);
    if(!pCom_Spd)
        return true;

    pCom_Spd->SetSpd(s);
    
    return false;
}













