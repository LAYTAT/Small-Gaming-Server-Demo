//
// Created by laylei on 2020/12/5.
//

#include "EventSystem.h"
#include "MsgID.pb.h"
#include "MSG_PLAYER_MOVE.pb.h"
#include "MSG_PLAYER_REGISTER.pb.h"
#include "SocketServer.h"
#include "GameSpec.pb.h"
#include "MySqlManager.h"
#include "CacheManager.h"

using namespace google::protobuf;

INSTANCE_SINGLETON(EventSystem)

EventSystem::EventSystem()
{
    m_msgHandler = nullptr;
    m_msgHandler = new MsgHandler<EventSystem>();
}

EventSystem::~EventSystem()
{

}

bool EventSystem::Init()
{
    // 连接到db server
//    SocketServer::Instance()->


    // 绑定处理函数
    m_msgHandler->RegisterMsg(MSGID::MSG_TEST_ID, &EventSystem::PlayerRegister);
    m_msgHandler->RegisterMsg(MSGID::MSG_PLAYER_MOVE_ID, &EventSystem::PlayerMove);
    m_msgHandler->RegisterMsg(MSGID::MSG_PLAYER_REGISTE_ID, &EventSystem::PlayerRegister);
    m_msgHandler->RegisterMsg(MSGID::MSG_ITEM_ADD, &EventSystem::PlayerAddItem);
    m_msgHandler->RegisterMsg(MSGID::MSG_PLAYER_LOGIN, &EventSystem::PlayerLogin);
    m_msgHandler->RegisterMsg(MSGID::MSG_ITEM_REMOVE, &EventSystem::PlayerRemoveItem);
    m_msgHandler->RegisterMsg(MSGID::MSG_RANKLIST_SHOW, &EventSystem::GetRankList);

    return true;
}

void EventSystem::Uinit()
{
    m_msgHandler->Uinit();
    delete m_msgHandler;
    m_msgHandler = nullptr;
}

INT32 EventSystem::PlayerRegister(const MesgInfo &stHead, const char *body, const INT32 len,const INT32 connfd)
{
    MSG_PLAYER_REGISTER msg_register;
    msg_register.ParseFromArray(body, len);

    INT32 ret = EntityMgr::Instance()->CreatePlayer(msg_register);
    if(ret == -1)
    {

        return ret;
    }

    SocketServer::Instance()->BroadCast(stHead, msg_register);
    return 0;
}

INT32 EventSystem::PlayerMove(const MesgInfo &stHead, const char *body, const INT32 len,const INT32 connfd)
{
    MSG_PLAYER_MOVE msg_move;
    if(!msg_move.ParseFromArray(body, len))
    {
        std::cout << "ParseFromArray MOVE failed" <<std::endl;
        return 1;
    }

    INT32 pid =stHead.uID;
    int x =( int) msg_move.x();
    int y =(int) msg_move.z();
    int r =(int) msg_move.ry();
    int s = 10;
    EntityMgr::Instance()->SetTrans(pid,x,y,r,s);
    SocketServer::Instance()->BroadCast(stHead, msg_move);

    return 0;
}

INT32 EventSystem::PlayerAddItem(const MesgInfo &stHead,const char *body, const INT32 len,const INT32 connfd)
{
    std::cout << "msgid " << stHead.msgID << std::endl;
    std::cout << "uid " << stHead.uID << std::endl;
    std::cout << "lens " << stHead.packLen << std::endl;
    GameSpec::AddItemReq test;
    GameSpec::AddItemRsp rsp;
    rsp.set_errcode(GameSpec::ERROR_NO_ERROR);
    if(!test.ParseFromArray(body, len))
    {
        std::cout << "ParseFromArray add item failed" <<std::endl;
        rsp.set_errcode(GameSpec::ERROR_PARSE_FAILED);
        SocketServer::Instance()->BroadCast(stHead, rsp);
        return 1;
    }
    std::cout << "add item itemid = " << test.itemid() << std::endl;
    std::cout << "add item const = " << test.count() << std::endl;

    if(EntityMgr::Instance()->HasPlayer(stHead.uID) == false)
    {
        rsp.set_errcode(GameSpec::ERROR_AUH_ERROR);
    }
    else if(m_bagSystem.AddItem( EntityMgr::Instance()->GetEttyByPid(stHead.uID) ,test.itemid(), test.count()) == false)
    {
        std::cout << "Add Item to Player's bag failed!  Playerid = " << stHead.uID << std::endl;
        rsp.set_errcode(GameSpec::ERROR_ADD_ITEM_FAIL);
    }
    GameSpec::Players t_playerinfo;
    m_bagSystem.GetPlayerInfo(EntityMgr::Instance()->GetEttyByPid(stHead.uID), &t_playerinfo);
    MySqlMgr::Instance()->UpdatePlayerInfo(&t_playerinfo);
    SocketServer::Instance()->BroadCast(stHead, rsp);
    return 0;
}

INT32 EventSystem::PlayerRemoveItem(const MesgInfo &stHead,const char *body, const INT32 len,const INT32 connfd)
{
    std::cout << "msgid " << stHead.msgID << std::endl;
    std::cout << "uid " << stHead.uID << std::endl;
    std::cout << "lens " << stHead.packLen << std::endl;
    GameSpec::RemoveItemReq test;
    GameSpec::RemoveItemRsp rsp;
    rsp.set_errcode(GameSpec::ERROR_NO_ERROR);
    if(!test.ParseFromArray(body, len))
    {
        std::cout << "ParseFromArray add item failed" <<std::endl;
        rsp.set_errcode(GameSpec::ERROR_PARSE_FAILED);
        SocketServer::Instance()->BroadCast(stHead, rsp);
        return 1;
    }
    std::cout << "remove item itemid = " << test.itemid() << std::endl;
    std::cout << "remove item const = " << test.count() << std::endl;
    std::cout << "remove item pos = " << test.count() << std::endl;

    if(EntityMgr::Instance()->HasPlayer(stHead.uID) == false)
    {
        rsp.set_errcode(GameSpec::ERROR_AUH_ERROR);
    }
    else if(m_bagSystem.RemoveItem( EntityMgr::Instance()->GetEttyByPid(stHead.uID) ,test.itemid(), test.pos(), test.count()) == false)
    {
        std::cout << "Remove Item to Player's bag failed!  Playerid = " << stHead.uID << std::endl;
        rsp.set_errcode(GameSpec::ERROR_REV_ITEM_FAIL);
    }
    GameSpec::Players t_playerinfo;
    m_bagSystem.GetPlayerInfo(EntityMgr::Instance()->GetEttyByPid(stHead.uID), &t_playerinfo);
    MySqlMgr::Instance()->UpdatePlayerInfo(&t_playerinfo);

    SocketServer::Instance()->BroadCast(stHead, rsp);
    return 0;
}

INT32 EventSystem::PlayerLogin(const MesgInfo &stHead, const char *body,const INT32 len,const INT32 connfd)
{
    //std::cout << "msgid " << stHead.msgID << std::endl;
    //std::cout << "uid " << stHead.uID << std::endl;
    //std::cout << "lens " << stHead.packLen << std::endl;
    GameSpec::CtlMsgLoginReq loginReq;
    GameSpec::CtlMsgLoginRsp rsp;
    if(!loginReq.ParseFromArray(body, len))
    {
        std::cout << "ParseFromArray palyer login_server failed" <<std::endl;
        rsp.set_errcode(GameSpec::ERROR_PARSE_FAILED);
        SocketServer::Instance()->BroadCast(stHead,rsp);
        return 1;
    }
    std::cout << "Redis login request name is : " << MySqlMgr::Instance()->GetPlayerId(loginReq.name()) << std::endl;
    std::cout << "login_server  uid = " << stHead.uID << std::endl;
    std::cout << "name = " << loginReq.name() << std::endl;
    std::cout << "password = " << loginReq.password() << std::endl;

    if(EntityMgr::Instance()->HasPlayer(stHead.uID) == false)
    {
        //取数据库
        //MySqlMgr::Instance()->GetPlayerInfo(stHead.uID,rsp.mutable_player());
        GameSpec::Players tptr ;
        MySqlMgr::Instance()->GetPlayerInfo(stHead.uID, &tptr);
        EntityMgr::Instance()->SetPlayer(tptr);

        //CacheManager::Instance()->newPlayer(tptr.id(), tptr.name(),tptr.exp());

        m_bagSystem.GetPlayerInfo(EntityMgr::Instance()->GetEttyByPid(stHead.uID),rsp.mutable_player());
    }
    else
    {
        std::cout << "start" << std::endl;
        m_bagSystem.GetPlayerInfo(EntityMgr::Instance()->GetEttyByPid(stHead.uID), rsp.mutable_player());
    }
    rsp.set_errcode(GameSpec::ERROR_NO_ERROR);

    SocketServer::Instance()->BroadCast(stHead, rsp);
    return 0;
}

INT32 EventSystem::GetRankList(const MesgInfo &stHead, const char *body,const INT32 len,const INT32 connfd)
{
    GameSpec::RankReq test;
    GameSpec::RankRsp rsp;
    if(!test.ParseFromArray(body, len))
    {
        std::cout << "ParseFromArray palyer login_server failed" <<std::endl;
        rsp.set_errcode(GameSpec::ERROR_PARSE_FAILED);
        SocketServer::Instance()->BroadCast(stHead,rsp);
        return 1;
    }

    auto t_ranklist = CacheManager::Instance()->getTopNPlayer(test.end());
    std::cout << "cout ranklist" << std::endl;
    for(auto it = t_ranklist.begin();it!=t_ranklist.end();it++)
    {
        std::cout << "name = "<<it->first <<std::endl;
        rsp.add_name(it->first);
        rsp.add_exp(it->second);
    }

    std::cout << "msgid " << stHead.msgID << std::endl;
    std::cout << "uid " << stHead.uID << std::endl;
    std::cout << "lens " << stHead.packLen << std::endl;
    std::cout << rsp.name_size()<<std::endl;
    rsp.set_errcode(GameSpec::ERROR_NO_ERROR);


    SocketServer::Instance()->BroadCast(stHead, rsp);
    return 1;
}
