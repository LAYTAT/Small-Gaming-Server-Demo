//
// Created by laylei on 2020/12/5.
//

#include "EventSystem.h"
#include "MsgID.pb.h"
#include "SocketServer.h"
#include "GameSpec.pb.h"
#include "MsgServerID.pb.h"
#include "Msg_To_And_From_DB.pb.h"
#include "MySqlManager.h"
#include "EntityMgr.h"
#include "MSG_PLAYER_MOVE.pb.h"


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
    // 绑定处理函数
    m_msgHandler->RegisterMsg(  MSGID::MSG_REQUEST_BAG_ITEMS_FROM_USER,  &EventSystem::PlayerReqItems);
    // 处理玩家移动
    m_msgHandler->RegisterMsg(MSGID::MSG_PLAYER_MOVE_ID, &EventSystem::PlayerMove);
    m_msgHandler->RegisterMsg(MSGID::MSG_PLAYER_REGISTE_ID, &EventSystem::PlayerRegister);

    return true;
}

void EventSystem::Uinit()
{
    m_msgHandler->Uinit();
    delete m_msgHandler;
    m_msgHandler = nullptr;
}

INT32 EventSystem::PlayerRegister(const MesgInfo &stHead, const char *body, const INT32 len,const INT32 connfd){
    MSG_PLAYER_REGISTER msgPlayerRegister;
    if(!msgPlayerRegister.ParseFromArray(body,len)) {
        std::cout << "game_server parsing register req failed" <<std::endl;
        return -1;
    }
    std::cout << "Unity Client Register Success. playerID: " << msgPlayerRegister.playerid() << " x = " << msgPlayerRegister.x() << std::endl;
}

// 验证用户的账号数据数据，数据由login server 发送过来
INT32 EventSystem::PlayerReqItems(const MesgInfo &stHead, const char *body, const INT32 len,const INT32 connfd)
{
    std::cout << "Enter Player request Processing" << std::endl;
    GameSpec::ClientReq clientReq;
    GameSpec::CtlMsgLoginRsp rsp;

    MesgInfo* msgInfo = new MesgInfo();
    msgInfo->msgID = MSGID::MSG_REPLY_BAG_ITEMS_FROM_GAME;
    msgInfo->uID = stHead.uID;

    if(!clientReq.ParseFromArray(body, len))
    {
        std::cout << "game_server parsing login req failed" <<std::endl;
        rsp.set_errcode(GameSpec::ERROR_PARSE_FAILED);
        msgInfo->packLen = clientReq.ByteSizeLong();
        SocketServer::Instance()->BroadCast(*msgInfo,rsp);
        return 1;
    }
    std::cout << "client uid = " << stHead.uID << std::endl;
    std::cout << "name = " << clientReq.username() << std::endl;
    std::cout << "session code = " << clientReq.session_code() << std::endl;

    // todo: 抽离这部分的部分逻辑到db server
    // 取玩家背包
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
        std::cout << "m_bagSystem.GetPlayerInfo(EntityMgr::Instance()->GetEttyByPid(stHead.uID), rsp.mutable_player())" << std::endl;
        m_bagSystem.GetPlayerInfo(EntityMgr::Instance()->GetEttyByPid(stHead.uID), rsp.mutable_player());
    }
    rsp.set_errcode(GameSpec::ERROR_NO_ERROR);


    msgInfo->packLen = rsp.ByteSizeLong();
    SocketServer::Instance()->BroadCast(*msgInfo, rsp);
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
    int ry =(int) msg_move.ry();
    int state = msg_move.state();
    std::cout << "User ID:"<< pid <<"state update : x=" << x << " z=" << y << " ry=" << ry << " state binary=" << state << std::endl;
    EntityMgr::Instance()->SetTrans(pid,x,y,ry,state);
    SocketServer::Instance()->BroadCast(stHead, msg_move);

    return 0;
}