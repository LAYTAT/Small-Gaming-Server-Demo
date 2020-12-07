//
// Created by laylei on 2020/12/5.
//
#include "SocketServer.h"
#include "MsgID.pb.h"
#include "PlayerManager.h"
#include "EventSystem.h"

INSTANCE_SINGLETON(SocketServer);

SocketServer::SocketServer()
{
    this->m_ListenSock = new baselink();
    this->m_ListenSock_DB = new baselink();
    this->m_msg_head = new MesgHead();
}

SocketServer::~SocketServer()
{
    delete m_msg_head;
    delete m_ListenSock;
    delete m_ListenSock_DB;
}

bool SocketServer::Init()
{
    // base socket init
    if (!m_ListenSock->Init(-1))
    {
        return false;
    }
    if (!m_ListenSock_DB->Init(-1))
    {
        return false;
    }
    if (m_ListenSock->OpenServer(LOGIN_SERVER_PORT, LOGIN_SERVER_IP_ADDR) == -1)
    {
        std::cout << "login server: Open server failed!" << std::endl;
        return false;
    }
    m_basefd = m_ListenSock->GetFD();
    //epoll
    if(m_epoll.Init() == false)
    {
        std::cout << "login server error: Epoll add ev failed!" << std::endl;
        return false;
    }
    if (m_epoll.EpollAdd(m_basefd) < 0) {
        std::cout << "\"Epoll add m_basefd failed" << std::endl;
    }

    m_msg_head->Init(1,1,1);
    return true;
}

void SocketServer::Uinit()
{
    m_epoll.Uinit();
    //析构函数中析构
    m_msg_head->Uinit();
    m_ListenSock->Uinit();
}

void SocketServer::Dojob()
{
    std::cout << "login server: Server start!" << std::endl;
    while (true)
    {
        INT32 ready_size = m_epoll.EpollWait();
        if(ready_size < 0){
            std::cout << "ready_size < 0 " << std::endl;
            break;
        }
        for (int i = 0; i < ready_size; i++)
        {
            struct epoll_event* t_epev = m_epoll.GetEvent(i);
            INT32 rdyfd = t_epev->data.fd;
            if (rdyfd == m_basefd) //新的链接，将新的socket加入epfd
            {
                int connfd = m_ListenSock->AcceptSocket();
                if (connfd == -1)
                {
                    std::cout <<"Accept socket failed ! connfd == -1" <<std::endl;
                    continue;
                }
                if(m_epoll.EpollAdd(connfd) == -1)
                {
                    std::cout << "Epoll add connfd failed!" << connfd << std::endl;
                    continue;
                }
                std::cout << "New client has linked! sockfd = " << connfd << std::endl;
            }
            else
            {
                baselink* t_linker = m_epoll.GetLinkerByfd(rdyfd);
                if (t_linker == nullptr)
                {
                    std::cout << "Can't find client socket in linkmap, connfd = "<< rdyfd << std::endl;
                    continue;
                }
                INT32 ret = t_linker->RecvData();
                if (ret == 0)
                {
                    continue;
                }
                else if (ret == -1)
                {
                    m_epoll.EpollRemove(rdyfd);
                    // 前面已经确认过connfd一定存在
                    continue;
                }

                INT32 t_packlen = t_linker->GetPackLens();
                while ( t_packlen != -1 )
                {
                    //获得一整个包，注意这里才改变buffer的头指针
                    char *str = t_linker->GetPack(t_packlen);
                    //判断第一个包头的信息
                    const MesgInfo t_msginfo =t_linker->GetMsginfo();
                    //std::cout << "t_mgsinfo  msgid = "<<t_msginfo.msgID << std::endl;
                    //std::cout << "t_mgsinfo  uid = "<<t_msginfo.uID << std::endl;
                    //std::cout << "t_mgsinfo  packlen = "<<t_msginfo.packLen << std::endl;
                    if(t_msginfo.msgID == MSGID::MSG_TEST_ID)
                    {
                        MsgTest t_msg;
                        t_msg.ParseFromArray( str, t_msginfo.packLen );
                        std::cout << "protobuf receive: --------------------------" << std::endl;
                        std::cout << "msgid: " << t_msg.id() << std::endl;
                        std::cout << "uid: " << t_msg.uid() << std::endl;
                        std::cout << "msg: " << t_msg.msg() << std::endl;
                        std::cout << "protobuf receive: --------------------------" << std::endl;
                        m_epoll.SendMsg(t_msginfo, t_msg, rdyfd);
                    }
                    else
                    {
                        //获得msgID绑定的函数，然后来处理对应的MSG
                        auto func = EventSystem::Instance()->GetMsgHandler()->GetMsgFunc(t_msginfo.msgID);
                        if (NULL != func)
                        {
                            //调用处理函数
                            (EventSystem::Instance()->*func)(t_msginfo, str, t_msginfo.packLen, rdyfd);
                        }
                    }
                    t_packlen = t_linker->GetPackLens();
                }
            }
        }
    }
}

INT32 SocketServer::ConnectDBServer() {
    INT32 fd_2_db_temp = m_ListenSock_DB->ConnectDBServer();
    if (fd_2_db_temp < 0) {
        std::cout << "connect to db server failed" << std::endl;
        return 0;
    }
    //    // 添加listen db的socketfd 到epoll；
    if (m_epoll.EpollAdd( m_ListenSock_DB->GetFD()) < 0) {
        std::cout << "Epoll add fd_to_db failed \n" << std::endl;
        return 0;
    } else {
        std::cout << "Epoll add fd_to_db success \n" << std::endl;
    }
    return fd_2_db_temp;
}

