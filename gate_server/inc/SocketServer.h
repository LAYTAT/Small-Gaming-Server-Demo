//
// Created by laylei on 2020/12/5.
//

#ifndef MY_SERVER_SOCKETSERVER_H
#define MY_SERVER_SOCKETSERVER_H

#include "Config.h"
#include "Singleton.h"
#include "../../db_server/inc/baselink.h"
#include "../../db_server/inc/Epoll.h"
#include "../../db_server/inc/MesgHead.h"

class SocketServer
{
private:
    SocketServer();
    ~SocketServer();
DECLARE_SINGLETON(SocketServer)
public:
    bool Init();
    void Uinit();
    void Dojob();
    inline void BroadCast(const MesgInfo& msghead, Message& msg){ m_epoll.BroadCast(msghead, msg); }
    inline void SendMsg(const MesgInfo& msghead, Message& msg, const INT32 connfd){ m_epoll.SendMsg(msghead, msg, connfd); }
private:
    baselink* m_ListenSock;
    INT32  m_basefd;
    Epoll m_epoll;
    MesgHead* m_msg_head;
};



#endif //MY_SERVER_SOCKETSERVER_H
