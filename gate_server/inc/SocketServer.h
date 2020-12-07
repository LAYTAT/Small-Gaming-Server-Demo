//
// Created by laylei on 2020/12/5.
//

#ifndef MY_SERVER_SOCKETSERVER_H
#define MY_SERVER_SOCKETSERVER_H

#include "Config.h"
#include "Singleton.h"
#include "baselink.h"
#include "Epoll.h"
#include "MesgHead.h"

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
    inline void SendMsgToGameServer(const MesgInfo& msghead, Message& msg){ std::cout << "Gate Sending request to Game"  << std::endl; m_epoll.SendMsg(msghead, msg , m_ListenSock_Game->GetFD()); }
    INT32 ConnectGameServer();

private:
    baselink* m_ListenSock;
    INT32  m_basefd;
    Epoll m_epoll;
    MesgHead* m_msg_head;

    baselink* m_ListenSock_Game;
    int which_game_server;
    int total_game_server_nums = 2;
};



#endif //MY_SERVER_SOCKETSERVER_H
