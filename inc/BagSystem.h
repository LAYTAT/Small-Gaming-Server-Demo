#ifndef _BAGSYSTEM_H_
#define _BAGSYSTEM_H_

#include "Config.h"
#include "Entity.h"
#include "ItemComponent.h"
#include "ItemsMgr.h"
#include "GameSpec.pb.h"

class BagSystem
{
public:
    BagSystem();
    ~BagSystem();

    bool AddItem(Entity* enty, INT32 itemid, INT32 cnt = 1);  // 向容器中添加一个道具 
    bool RemoveItem(Entity* enty, INT32 itemid, INT32 pos, INT32 cnt);   // 使用容器中的道具或是消耗一定数量的道具
    void GetBagInfo(Entity* enty, GameSpec::BagInfo *baginfo);  // 获取玩家的背包信息
    void GetPlayerInfo(Entity* enty, GameSpec::Players* plyerinfo);  // 获取一个用户的所有信息
};

#endif
