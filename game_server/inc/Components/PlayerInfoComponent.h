#ifndef _PLAYERINFOCOMPONENT_H_
#define _PLAYERINFOCOMPONENT_H_

#include "Config.h"
#include "BaseComponent.h"

class PlayerInfoComponent : public BaseComponent
{
public :
    PlayerInfoComponent(){};
    ~PlayerInfoComponent(){};
    FUNCCLONE(PlayerInfoComponent);

    bool Init(INT32 pid,std::string name,INT32 rank,INT32 exp)
    {
        this->pid = pid;
        this->name = name;
        this->rank = rank;
        this->exp = exp;
        return true;
    }   
    void Uinit()
    {

    }
    
    INT32 pid;
    std::string name;
    INT32 rank;
    INT32 exp;

};

#endif
