#ifndef _ITEMSMGR_H_
#define _ITEMSMGR_H_

// 所有Item的注册类
// 方便查找itemid对应的type
// 方便程序new一个对应的item

#include "Config.h"
#include "Singleton.h"
#include "ItemBase.h"


class ItemsMgr
{
private:
    ItemsMgr(){}
    ~ItemsMgr(){}
    DECLARE_SINGLETON(ItemsMgr)

public:
    bool Init();
    void Uinit();
    
    INT32 GetTypeByID(INT32 itemid);
    BagItem* GetBagItem(INT32 itemid, INT32 cnt); // 获得一个BagItem对象
    EquipItem* GetEquipItem(INT32 itemid); // 获得一个EquipItem对象
    CurrencyItem* GetCurrencyItem(INT32 itemid,INT32 cnt);

private:
    std::unordered_map<INT32, INT32> m_itemRegister;  
    


};

#endif
