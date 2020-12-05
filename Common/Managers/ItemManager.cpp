//
// Created by laylei on 2020/12/5.
//

#include "ItemsMgr.h"
#include "ItemList.h"

INSTANCE_SINGLETON(ItemsMgr)

bool ItemsMgr::Init()
{
    // item×¢²ábiao

    m_itemRegister[ITEMID_REDPOTION] = ITEMTYPE_BAG_ITEM; //ºìÒ©Ë® - bagitem

    m_itemRegister[ITEMID_SWORD] = ITEMTYPE_EQUIP_ITEM;  //½£ - equipitem

    m_itemRegister[ITEMID_COIN] = ITEMTYPE_CURRENCY_ITEM;  //½ð±Ò - currencyitem

    return true;
}

void ItemsMgr::Uinit()
{

}

INT32 ItemsMgr::GetTypeByID(INT32 itemid)
{
    return m_itemRegister[itemid];
}

//todo ÓÃ±íÅÐ¶Ï
BagItem* ItemsMgr::GetBagItem(INT32 itemid, INT32 cnt)
{
    //if(m_itemRegister.find(itemid) == m_itemRegister.end())
    //{
    //    return nullptr;
    //}
    return new BagItem(itemid, m_itemRegister[itemid], cnt, false);   // itemid , typeid, cnt, binded
}

EquipItem* ItemsMgr::GetEquipItem(INT32 itemid)
{
    //if(m_itemRegister.find(itemid) == m_itemRegister.end())
    //{
    //    return nullptr;
    //}
    return new EquipItem(itemid, m_itemRegister[itemid], false, 90, 100);   // itemid ,type,binded,attack,durable
}

CurrencyItem* ItemsMgr::GetCurrencyItem(INT32 itemid,INT32 cnt)
{
    //if(m_itemRegister.find(itemid) == m_itemRegister.end())
    //{
    //    return nullptr;
    //}
    return new CurrencyItem(itemid, m_itemRegister[itemid], cnt);   // itemid ,type,binded,attack,durable
}
