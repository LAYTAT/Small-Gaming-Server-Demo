//
// Created by laylei on 2020/12/5.
//

#ifndef MY_SERVER_COMPONENTSMGR_H
#define MY_SERVER_COMPONENTSMGR_H

#include "Config.h"
#include "Singleton.h"
#include "BaseComponent.h"

class ComponentsMgr
{
private:
    ComponentsMgr();
    ~ComponentsMgr();
DECLARE_SINGLETON(ComponentsMgr)
public:
    bool Init();
    void Uinit();
    BaseComponent* GetComponent(INT32 comtypeid);


private:
    std::unordered_map<INT32, BaseComponent*> m_ComponentsMap;

};

#endif //MY_SERVER_COMPONENTSMGR_H
