#ifndef _BAGCONTAINER_H_
#define _BAGCONTIANER_H_

#include "Config.h"
#include "ItemContainer.h"
#include "ItemBase.h" 

class BagContainer : public ItemContainer
{
public:
    BagContainer(){}
    ~BagContainer(){}

    bool AddItem(INT32 itemid,INT32 cnt)  
    {
        int pos = -1;  // 当要新占用一个格子时，这个格子的位置
        int t_len = m_container.size();
        //std::cout << "t_len  = "<<t_len<<std::endl;
        //遍历背包，向里面加入item
        for(int i=0;i<t_len;i++)
        {
            if(m_container[i] == nullptr)
            {
                if(pos == -1) pos = i;
                continue;
            }
            if(m_container[i]->GetID() == itemid)
            {
                cnt = m_container[i]->AddItem(cnt);
                
                if(cnt == 0)
                {
                    break;
                }
            }
        }
        if(cnt == 0)
        {
            return true;
        }
        if(pos != -1)
        {
            std::cout <<"pos = "<<pos<< "   insert into bag "<< itemid << "   "<<cnt<<std::endl;
            m_container[pos] = ItemsMgr::Instance()->GetBagItem(itemid, cnt);            
            return true;
        }
        return false;
    }


    void RemoveItem(INT32 num, INT32 rcount)   // rcount = removecount; rcount == -1 means remove all item
                                    // if you want to rm a equipment,you can use rcount = -1 to remove 
                                    // this equipment
    {
        if(num >= MAX_BAG_SIZE || m_container[num] == nullptr)
        {
            return; 
        }

        if(m_container[num]->RemoveItem(rcount) == false)
        {
            // 这个道具的数量<=0，移除这个格子
            delete m_container[num];
            m_container[num] = nullptr;
        }
        return;
    }

};

#endif
