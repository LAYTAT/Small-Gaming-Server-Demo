//
// Created by laylei on 2020/12/5.
//

#include "EventSystem.h"
#include "MsgID.pb.h"
#include "SocketServer.h"
#include "GameSpec.pb.h"
#include "MySqlManager.h"
#include "CacheManager.h"
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
    // 绑定处理函数
    m_msgHandler->RegisterMsg( MSGID::MSG_LOGIN_REQUEST_DB, &EventSystem::PlayerAuth);

    return true;
}

void EventSystem::Uinit()
{
    m_msgHandler->Uinit();
    delete m_msgHandler;
    m_msgHandler = nullptr;
}

// 验证用户的账号数据数据，数据由login server 发送过来
INT32 EventSystem::PlayerAuth(const MesgInfo &stHead, const char *body, const INT32 len,const INT32 connfd)
{
    std::cout << "Enter Player Authentication Process" << std::endl;
    DbReq_User_Auth dbReqUserAuth;
    DbRep_User_Auth* rsp = new DbRep_User_Auth();

    if(!dbReqUserAuth.ParseFromArray(body, len))
    {
        std::cout << "db_server parsing login req failed" <<std::endl;
        rsp->set_errcode(GameSpec::ERROR_PARSE_FAILED);
        rsp->set_isuserverified(false);
        SocketServer::Instance()->BroadCast(stHead,*rsp);
        return 1;
    }

    std::cout << "login_server  uid = " << stHead.uID << std::endl;
    std::cout << "name = " << dbReqUserAuth.hashedusrid() << std::endl;
    std::cout << "password = " << dbReqUserAuth.hashedusrpwd() << std::endl;

    if(MySqlMgr::Instance()->GetPlayerId(dbReqUserAuth.hashedusrid()) < 0) {
        // 数据库验证 没有通过
        rsp->set_errcode(GameSpec::ERROR_SEARCH_FAIL);
        rsp->set_isuserverified(false);

        MesgInfo* reply_to_login = new MesgInfo();
        reply_to_login->msgID = MSGID::MSG_LOGIN_AUTH_PASSED;
        reply_to_login->uID = stHead.uID;
        reply_to_login->packLen = rsp->ByteSizeLong();
        SocketServer::Instance()->BroadCast(*reply_to_login, *rsp);
        delete reply_to_login;

        std::cout << "login_server login req failed !" << stHead.uID << std::endl;

        return -1;
    }
//    std::cout << "Does User Id exists: "<< MySqlMgr::Instance()->GetPlayerId(dbReqUserAuth.hashedusrid())  << std::endl;


    // 数据库验证通过
    rsp->set_errcode(GameSpec::ERROR_NO_ERROR);
    rsp->set_isuserverified(true);

    MesgInfo* reply_to_login = new MesgInfo();
    reply_to_login->msgID = MSGID::MSG_LOGIN_AUTH_PASSED;
    reply_to_login->uID = stHead.uID;
    reply_to_login->packLen = rsp->ByteSizeLong();
    SocketServer::Instance()->BroadCast(*reply_to_login, *rsp);
    delete reply_to_login;

    std::cout << "login_server login req passed! UID:" << stHead.uID << std::endl;

    return 0;
}