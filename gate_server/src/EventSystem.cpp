//
// Created by laylei on 2020/12/5.
//

#include "EventSystem.h"
#include "MsgID.pb.h"
#include "SocketServer.h"
#include "GameSpec.pb.h"
#include "MsgServerID.pb.h"
#include "Msg_To_And_From_DB.pb.h"

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
    // 连接到game server
    SocketServer::Instance()->ConnectGameServer();

    // 绑定处理函数
    m_msgHandler->RegisterMsg(MSGID::MSG_REQUEST_BAG_ITEMS_FROM_USER,  &EventSystem::PlayerReqItems);

    return true;
}

void EventSystem::Uinit()
{
    m_msgHandler->Uinit();
    delete m_msgHandler;
    m_msgHandler = nullptr;
}

// 验证用户的账号数据数据，数据由login server 发送过来
INT32 EventSystem::PlayerReqItems(const MesgInfo &stHead, const char *body, const INT32 len,const INT32 connfd)
{
    std::cout << "Enter Player Reqesting Items Process in Gate Server." << std::endl;

    GameSpec::ClientReq clientReq;
    GameSpec:: rsp;
//
//    if(!dbReqUserAuth.ParseFromArray(body, len))
//    {
//        std::cout << "db_server parsing login req failed" <<std::endl;
//        rsp.set_errcode(GameSpec::ERROR_PARSE_FAILED);
//        rsp.set_isuserverified(false);
//        SocketServer::Instance()->BroadCast(stHead,rsp);
//        return 1;
//    }
//    std::cout << "Redis login request name is : " << MySqlMgr::Instance()->GetPlayerId(loginReq.name()) << std::endl;
//    std::cout << "login_server  uid = " << stHead.uID << std::endl;
//    std::cout << "name = " << loginReq.name() << std::endl;
//    std::cout << "password = " << loginReq.password() << std::endl;
//
//    if(EntityMgr::Instance()->HasPlayer(stHead.uID) == false)
//    {
//        //取数据库
//        //MySqlMgr::Instance()->GetPlayerInfo(stHead.uID,rsp.mutable_player());
//        GameSpec::Players tptr ;
//        MySqlMgr::Instance()->GetPlayerInfo(stHead.uID, &tptr);
//        EntityMgr::Instance()->SetPlayer(tptr);
//
//        //CacheManager::Instance()->newPlayer(tptr.id(), tptr.name(),tptr.exp());
//
//        m_bagSystem.GetPlayerInfo(EntityMgr::Instance()->GetEttyByPid(stHead.uID),rsp.mutable_player());
//    }
//    else
//    {
//        std::cout << "start" << std::endl;
//        m_bagSystem.GetPlayerInfo(EntityMgr::Instance()->GetEttyByPid(stHead.uID), rsp.mutable_player());
//    }
//    rsp.set_errcode(GameSpec::ERROR_NO_ERROR);
//
//    SocketServer::Instance()->BroadCast(stHead, rsp);
    return 0;
}