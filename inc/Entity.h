//
// Created by laylei on 2020/12/5.
//

#ifndef MY_SERVER_ENTITY_H
#define MY_SERVER_ENTITY_H

#include "Config.h"
#include "BaseComponent.h"
#include "ComponentsMgr.h"

class Entity
{
public:
    Entity(INT32 id){ m_id = id; m_components.clear(); return; }
    ~Entity()
    {
        // 初始化map
        for(auto it = m_components.begin(); it != m_components.end(); it++)
        {
            delete it->second;
            it->second = nullptr;
        }
        m_components.clear();
        return;
    }
    //void SetID(INT32 id) { m_id = id; return; }
    INT32 GetID() { return m_id; }
    bool AddComponent(INT32 comtypeID)
    {
        // 查看是否已经拥有这个组件
        if(m_components.find(comtypeID) != m_components.end())
        {
            std::cout << "components has alreader, comtypeid = "<< comtypeID << std::endl;
            return false;
        }
        // 添加组件
        m_components[comtypeID] = ComponentsMgr::Instance()->GetComponent(comtypeID);
        // 判空
        if(m_components[comtypeID] == nullptr)
        {
            std::cout << "m_components[] == null " << std::endl;
            return false;
        }
        return true;
    }
    BaseComponent* GetComponent(INT32 comtypeID)
    {
        return m_components[comtypeID];
    }
private:
    INT32 m_id;
    std::unordered_map<INT32, BaseComponent*> m_components;

};

#endif //MY_SERVER_ENTITY_H
