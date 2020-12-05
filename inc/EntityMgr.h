//
// Created by laylei on 2020/12/5.
//

#ifndef MY_SERVER_ENTITYMGR_H
#define MY_SERVER_ENTITYMGR_H

#include "TransformSystem.h"
#include "Entity.h"
#include "Singleton.h"
#include "MSG_PLAYER_REGISTER.pb.h"
#include "GameSpec.pb.h"
using namespace google::protobuf;

class EntityMgr
{
private:
    EntityMgr();
    ~EntityMgr();
DECLARE_SINGLETON(EntityMgr)
public:
    bool Init();
    void Uinit();

    //todo！
    // 创建玩家实体
    INT32 CreatePlayer(MSG_PLAYER_REGISTER& msg);
    // 获得实体
    Entity* GetEttyByPid(INT32 pid);
    // 设置玩家位置
    bool SetTrans(INT32 pid,int x,int y,int r,int s);
    // 判断是否有玩家
    bool HasPlayer(INT32 pid){ if(m_PlayerMap.find(pid) != m_PlayerMap.end()){return true;} return false; }
    // 从数据库读出一个玩家信息
    bool SetPlayer(GameSpec::Players ply);


private:
    std::unordered_map<INT32, Entity*> m_EntityMap;  //entityid -> entity 实体map
    std::unordered_map<INT32, INT32> m_PlayerMap;   //playerid -> entityid 玩家集合，映射到整个实体的map
    // 实体的注册列表（有什么components）
    std::unordered_map<INT32, std::vector<INT32>> m_CreateEntityMap; // entity type id -> components[]

    TransformSystem trans; // 位移System
    INT32 m_eidpos; // 代表当前创建的entity的id，这个要优化
};
#endif //MY_SERVER_ENTITYMGR_H
