#ifndef _EQUIPCONTAINER_H_
#define _EQUIPCONTIANER_H_

#include "Config.h"
#include "ItemContainer.h"
#include "ItemBase.h" 

class EquipContainer : public ItemContainer
{
public:
    EquipContainer(){}
    ~EquipContainer(){}

    bool AddItem(INT32 itemid)  
    {
        INT32 pos = -1;
        INT32 t_len = m_container.size();
        // 找到一个空格来放装备
        for(int i=0;i<t_len;i++)
        {
            if(m_container[i] == nullptr)
            {
                if(pos == -1) pos = i;
                break;
            }
        }
        if(pos == -1) return false;
        m_container[pos] = ItemsMgr::Instance()->GetEquipItem(itemid);

        return true;
    }


    void RemoveItem(INT32 num)
    {
        if(m_container[num] == nullptr)
        {
            return;
        }
        delete m_container[num];
        m_container[num] = nullptr;

        return;
    }

};

#endif
