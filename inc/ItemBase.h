#ifndef _ITEMBASE_H_
#define _ITEMBASE_H_
//道具相关基类
#include "Config.h"

class ItemBase   // 道具基类，保存公共的属性
{
public:
    ItemBase(INT32 itemid, INT32 type)
    {
        this->itemID = itemid;
        this->typeID = type;
    }
    virtual ~ItemBase() = default;
    virtual INT32 AddItem(INT32 count) = 0;   //添加物品，返回剩下没有加入的数量
    virtual bool RemoveItem(INT32 count) = 0;  // true代表剩下物品数量大于0; false代表这件物品已经被用完了;
    INT32 GetType(){ return typeID; }
    INT32 GetID(){ return itemID; }

    INT32 itemID; // itemid = 这个道具的id
    INT32 typeID; // typeid = 装备、材料、货币任务道具等
    INT32 count;  // 数量
    INT32 overly; // 最大叠加数量,暂时在util.h中配置
};

class BagItem : public ItemBase   // 背包中的道具类，包括材料，消耗品等
{
public:
    BagItem(INT32 itemid,INT32 type,INT32 count,bool binded)
    :ItemBase(itemid,type)
    {
       this->count = count;
       this->binded = binded;
       this->overly = MAX_ITEM_BAGOVERLY;  //最大叠加数量、在util.h中定义
    }

    INT32 AddItem(INT32 cnt)
    {
        if(this->count + cnt > MAX_ITEM_BAGOVERLY)
        {
            cnt -= MAX_ITEM_BAGOVERLY - this->count;
            this->count = MAX_ITEM_BAGOVERLY;
            return cnt;
        }
        else
        {
            this->count += cnt;
        }
        return 0;
    }
    bool RemoveItem(INT32 count)
    {
        this->count -= count;
        if(this->count <= 0)
        {
            this->count = 0;
            return false;
        }
        return true;
    }


    bool binded;   // 是否绑定在人物身上（是否可交易）


};

class EquipItem : public ItemBase  // 装备道具类 
{
public:
    EquipItem(INT32 itemid,INT32 type,bool binded,INT32 attack,INT32 durability)
    :ItemBase(itemid,type)
    {
        this->binded = binded;
        this->attack = attack;
        this->durability = durability;
        this->count = 1;
        this->overly = 1;
    }
    ~EquipItem(){}

    INT32 AddItem(INT32 count)
    {
        // 装备类直接new就可以了
        // 不需要再使用additem
        return count;
    }
    bool RemoveItem(INT32 count)
    { 
        //装备移除同理
        return false;
    }
    bool binded;   // 是否绑定在人物身上（是否可交易）
    INT32 attack;   // 攻击力
    INT32 durability;  // 耐久度


};

class CurrencyItem : public ItemBase  // 货币基类
{
public:
    CurrencyItem(INT32 itemid,INT32 type,INT32 count)
    :ItemBase(itemid,type)
    {
        this->count = count;
        this->overly = MAX_ITEM_CURRENCYOVERLY;  
    }
    ~CurrencyItem(){}
    INT32 AddItem(INT32 cnt)
    {
        if(this->count + cnt > MAX_ITEM_CURRENCYOVERLY)
        {
            cnt -= MAX_ITEM_CURRENCYOVERLY - this->count;
            this->count = MAX_ITEM_CURRENCYOVERLY;
            return cnt;
        }
        else
        {
            this->count += cnt;
        }
        return 0;
    }
    bool RemoveItem(INT32 count)
    { 
        this->count -= count;
        if(this->count <= 0)
        {
            this->count = 0;
            return false;
        }
        return true;
    }
};

#endif
