#ifndef _TRANSFORMSYSTEM_H_
#define _TRANSFORMSYSTEM_H_

#include "Config.h"
#include "TransformComponent.h"
#include "Entity.h"

class TransformSystem
{
public:
    TransformSystem();
    ~TransformSystem();

    void UpdatePosition(const PositionComponent & pos,const OrientComponent & ori,const SpeedComponent & spe);

    bool SetTrans(Entity*,int x,int y,int r,int s);


};

#endif
