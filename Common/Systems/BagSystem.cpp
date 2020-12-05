#include "BagSystem.h"
#include "ItemList.h"
#include "PlayerInfoComponent.h"

BagSystem::BagSystem()
{

}

BagSystem::~BagSystem()
{

}

bool BagSystem::AddItem(Entity* enty, INT32 itemid, INT32 cnt)
{
    ItemContainerComponent* itemCont = (ItemContainerComponent*)enty->GetComponent(COMPONENTS_BAG);
    INT32 type = ItemsMgr::Instance()->GetTypeByID(itemid); 

    //todo: change add item type
    type = ITEMTYPE_BAG_ITEM;

    bool flag = true;
    switch(type){
        case ITEMTYPE_BAG_ITEM:
            flag = itemCont->m_bagContainer.AddItem(itemid, cnt);
            break;
        case ITEMTYPE_EQUIP_ITEM:
            flag = itemCont->m_equipContainer.AddItem(itemid);
            break;
        case ITEMTYPE_CURRENCY_ITEM:
            flag = itemCont->m_curContainer.AddItem(itemid, cnt);
            break;
        default:
            return false;
    }
    return flag;
}

bool BagSystem::RemoveItem(Entity* enty,INT32 itemid, INT32 pos, INT32 cnt)
{
    ItemContainerComponent* itemCont = (ItemContainerComponent*)enty->GetComponent(COMPONENTS_BAG);
    INT32 type = ItemsMgr::Instance()->GetTypeByID(itemid); 
    type = ITEMTYPE_BAG_ITEM;
    bool flag = true;
    switch(type){
        case ITEMTYPE_BAG_ITEM:
            itemCont->m_bagContainer.RemoveItem(pos, cnt);
            break;
        case ITEMTYPE_EQUIP_ITEM:
            itemCont->m_equipContainer.RemoveItem(pos);
            break;
        case ITEMTYPE_CURRENCY_ITEM:
            itemCont->m_curContainer.RemoveItem(pos,cnt);
            break;
        default:
            return false;
    }
    return flag;
}

void BagSystem::GetBagInfo(Entity* enty,GameSpec::BagInfo *baginfo)
{
    ItemContainerComponent* t_items = (ItemContainerComponent*)(enty->GetComponent(COMPONENTS_BAG));
    
    const std::vector<ItemBase*>& t_container_bag =  t_items->m_bagContainer.m_container;
    INT32 t_size = t_container_bag.size();
    for(INT32 i=0;i<t_size;i++)
    {
        BagItem* t_c = (BagItem*)t_container_bag[i];
         
        GameSpec::BagItem* t_cons = baginfo->add_bag();
        if(t_c == nullptr)
        {
            t_cons->set_itemid(-1);
            continue;
        }
        t_cons->set_itemid(t_c->itemID);
        t_cons->set_typeid_(t_c->typeID);
        t_cons->set_count(t_c->count);
        t_cons->set_overly(t_c->overly);
        t_cons->set_binded(t_c->binded);
    }
    const std::vector<ItemBase*>& t_container_equip = t_items->m_equipContainer.m_container;
    t_size = t_container_equip.size();
    for(INT32 i=0;i<t_size;i++)
    {
        EquipItem* t_c = (EquipItem*)t_container_equip[i];
        GameSpec::EquipItem* t_cons = baginfo->add_equipment();
        if(t_c == nullptr)
        {
            t_cons->set_itemid(-1);
            continue;
        }
        t_cons->set_itemid(t_c->itemID);
        t_cons->set_typeid_(t_c->typeID);
        t_cons->set_count(t_c->count);
        t_cons->set_overly(t_c->overly);
        t_cons->set_binded(t_c->binded);
        t_cons->set_attack(t_c->attack);
        t_cons->set_durability(t_c->durability);
    }
    const std::vector<ItemBase*>& t_container_currency = t_items->m_curContainer.m_container;
    t_size = t_container_currency.size();
    for(INT32 i=0;i<t_size;i++)
    {
        CurrencyItem* t_c = (CurrencyItem*)t_container_currency[i];
        GameSpec::CurrencyItem* t_cons = baginfo->add_currency();
        if(t_c == nullptr)
        {
            t_cons->set_itemid(-1);
            continue;
        }
        t_cons->set_itemid(t_c->itemID);
        t_cons->set_typeid_(t_c->typeID);
        t_cons->set_count(t_c->count);
        t_cons->set_overly(t_c->overly);
    }
    return ;
}

void BagSystem::GetPlayerInfo(Entity* enty,GameSpec::Players* playerinfo)
{
    PlayerInfoComponent* t_pinfo = (PlayerInfoComponent*)(enty->GetComponent(COMPONENTS_PLAYERINFO));
    playerinfo->set_id(t_pinfo->pid);
    playerinfo->set_name(t_pinfo->name);
    playerinfo->set_rank(t_pinfo->rank);
    playerinfo->set_rank(t_pinfo->exp);

    GameSpec::BagInfo* baginfo = playerinfo->mutable_bags();
    if(baginfo == nullptr)
    {
        std::cout << "baginfo == nullptr" << std::endl;
    }
    GetBagInfo(enty, baginfo);
    return ;
}


