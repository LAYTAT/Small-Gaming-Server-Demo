#ifndef _ITEMCONTAINER_H_
#define _ITEMCONTAINER_H_

#include "Config.h"
#include "ItemBase.h"
#include "ItemsMgr.h"

class ItemContainer  // 背包容器模板类
{
public:
    ItemContainer(){  // 初始化，直接申请背包格数相等的指针数量
        m_container.clear(); 
        m_container.resize(MAX_BAG_SIZE);
        for(int i=0;i<MAX_BAG_SIZE;i++)
        {
            m_container[i] = nullptr;
        }
    }
    virtual ~ItemContainer(){   // 记得delete
        int t_len = m_container.size();
        for(int i=0;i<t_len;i++)
        {
            if(m_container[i] != nullptr)
            {
                delete m_container[i];
            }
        }
    }
    std::vector<ItemBase*> m_container;

};

#endif
