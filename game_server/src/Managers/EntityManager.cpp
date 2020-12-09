//
// Created by laylei on 2020/12/5.
//

#include "EntityMgr.h"

#include "TransformComponent.h"
#include "PlayerInfoComponent.h"
#include "ItemComponent.h"
#include "TransformSystem.h"

INSTANCE_SINGLETON(EntityMgr)

EntityMgr::EntityMgr()
{
    m_EntityMap.clear();
    m_PlayerMap.clear();
    m_CreateEntityMap.clear();
}

EntityMgr::~EntityMgr()
{
    m_EntityMap.clear();
    m_PlayerMap.clear();
    m_CreateEntityMap.clear();
}

bool EntityMgr::Init()
{
    //实体组件注册表
    m_eidpos = 1000;
    //PlayerEntity
    m_CreateEntityMap[ENTITY_PLAYER].push_back(COMPONENTS_POSITION);
    m_CreateEntityMap[ENTITY_PLAYER].push_back(COMPONENTS_ORIENT);
    m_CreateEntityMap[ENTITY_PLAYER].push_back(COMPONENTS_SPEED);
    m_CreateEntityMap[ENTITY_PLAYER].push_back(COMPONENTS_BAG);
    m_CreateEntityMap[ENTITY_PLAYER].push_back(COMPONENTS_PLAYERINFO);

    //Entity...
    return true;
}

void EntityMgr::Uinit()
{
    for(auto it = m_EntityMap.begin(); it != m_EntityMap.end(); it++)
    {
        delete it->second;
        it->second = nullptr;
    }
    return;
}

//为了完成第三周作业，现废弃QAQ
INT32 EntityMgr::CreatePlayer(MSG_PLAYER_REGISTER& msg)
{

    if( m_PlayerMap.find(msg.playerid()) != m_PlayerMap.end() )
    {
        std::cout << "Player has already exist! Playerid = " << msg.playerid() << std::endl;
        return -1;
    }
    m_PlayerMap[msg.playerid()] = m_eidpos;
    Entity* tmp = new Entity(m_eidpos);
    std::vector<INT32>& vec = m_CreateEntityMap[ENTITY_PLAYER];
    for(INT32 i = 0; i < (INT32)vec.size();i++)
    {
        if(tmp->AddComponent(vec[i]) == false)
        {
            std::cout << "Player Entity add components failed!" << std::endl;
            return -1;
        }
    }

    //todo 优化成system根据id取出
    ((PositionComponent*)tmp->GetComponent(COMPONENTS_POSITION))->x = msg.x();
    ((PositionComponent*)tmp->GetComponent(COMPONENTS_POSITION))->z = msg.z();
    ((OrientComponent*)tmp->GetComponent(COMPONENTS_ORIENT))->ry = msg.ry();
    ((SpeedComponent*)tmp->GetComponent(COMPONENTS_SPEED))->speed = msg.speed();

    // --------
    m_EntityMap[m_eidpos] = tmp;
    m_eidpos++;

    std::cout << "Player register in server! Playerid = " << msg.playerid() << std::endl;
    return 0;
}

Entity* EntityMgr::GetEttyByPid(INT32 pid)
{
    auto eidIT = m_PlayerMap.find(pid);
    if(eidIT== m_PlayerMap.end())
        return nullptr;
    auto etty = m_EntityMap.find(eidIT->second);
    if(etty == m_EntityMap.end())
        return nullptr;
    return etty->second;
}

bool EntityMgr::SetTrans(INT32 pid,int x,int y,int r,int s)
{
    auto pE =GetEttyByPid(pid);
    if(!pE)
        return 1;
    trans.SetTrans(pE,x,y,r,s);

    return 0;
}

bool EntityMgr::SetPlayer(GameSpec::Players ply)
{
    if( m_PlayerMap.find(ply.id()) != m_PlayerMap.end() )
    {
        std::cout << "Player has already exist! Playerid = " << ply.id() << std::endl;
        return false;
    }
    //std::cout << "ply/id   == = = "<<ply.id() << std::endl;
    m_PlayerMap[ply.id()] = m_eidpos;
    Entity* tmp = new Entity(m_eidpos);
    std::vector<INT32>& vec = m_CreateEntityMap[ENTITY_PLAYER];
    for(INT32 i = 0; i < (INT32)vec.size();i++)
    {
        if(tmp->AddComponent(vec[i]) == false)
        {
            std::cout << "Player Entity add components failed!" << std::endl;
            return false;
        }
    }

    ((PlayerInfoComponent*)tmp->GetComponent(COMPONENTS_PLAYERINFO))->Init(ply.id(), ply.name(), ply.rank(), ply.exp());
    ItemContainerComponent* t_items = (ItemContainerComponent*)tmp->GetComponent(COMPONENTS_BAG);

    for(INT32 i = 0; i<ply.bags().bag_size();i++)
    {
        std::cout << "setplayer!!!  itemid = " << ply.bags().bag(i).itemid() << "  count "<<ply.bags().bag(i).count()<<std::endl;
        t_items->m_bagContainer.AddItem(ply.bags().bag(i).itemid(), ply.bags().bag(i).count());
    }

    for(INT32 i = 0; i<ply.bags().equipment_size();i++)
    {
        t_items->m_equipContainer.AddItem(ply.bags().equipment(i).itemid());
    }

    for(INT32 i = 0; i<ply.bags().currency_size();i++)
    {
        t_items->m_curContainer.AddItem(ply.bags().currency(i).itemid(), ply.bags().currency(i).count());
    }

    m_EntityMap[m_eidpos] = tmp;
    m_eidpos++;

    std::cout << "Player register in server! Playerid = " << ply.id() << std::endl;
    return true;
}

