#ifndef _TRANSFORMCOMPONENT_H_
#define _TRANSFORMCOMPONENT_H_

#include "Config.h"
#include "BaseComponent.h"

// enum = 1
class PositionComponent : public BaseComponent 
{
public:
    INT32 x,z;
    PositionComponent(){};
    ~PositionComponent(){};
    bool Init(INT32 x,INT32 y) 
    {
        this->x = x;
        this->z = y;
        return true;
    }

    void SetPos(int x,int y)
    {
        this->x =x;
        this->z =y;
    }
    FUNCCLONE(PositionComponent);
};
// enum = 2
class OrientComponent : public BaseComponent
{
public:
    INT32 ry;
    OrientComponent(){};
    ~OrientComponent(){};
    bool Init(INT32 y) 
    {
        this->ry = y;
        return true;
    }

    void SetOri(int r)
    {
        this->ry =r;
    }
    FUNCCLONE(OrientComponent);
};
// enum = 3
class SpeedComponent : public BaseComponent
{
public:
    INT32 speed;
    SpeedComponent(){};
    ~SpeedComponent(){};
    bool Init(INT32 speed){ this->speed = speed; return true; };

    void SetSpd(int s)
    {
        this->speed =s;
    }
    FUNCCLONE(SpeedComponent);
};

#endif
