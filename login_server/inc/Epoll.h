#ifndef _EPOLL_H_
#define _EPOLL_H_

#include "Config.h"
#include "baselink.h"
#include "MsgTest.pb.h"
using namespace google::protobuf;

class Epoll 
{
public :
    Epoll();
    ~Epoll();
    bool Init();
    void Uinit();
    INT32 EpollAdd(INT32 connfd);  //添加连接
    INT32 EpollWait(INT32 itemout = -1);  //封装的epoll_wait
    struct epoll_event* GetEvent(INT32 pos);  //获取响应的时间
    baselink* GetLinkerByfd(INT32 connfd);  //获取baselink
    void EpollRemove(INT32 connfd);  //删除一个连接
    void BroadCast(const MesgInfo& msghead, Message& msg);  //广播消息
    void SendMsg(const MesgInfo& msghead, Message& msg, const INT32 connfd); //发送消息给某个用户
    INT32 connfd_db_server;

private:
    INT32 m_epfd;
    struct epoll_event m_ev;
    struct epoll_event* m_events;
    std::unordered_map<INT32, baselink*> m_linkmap;

};
#endif
