#include "Epoll.h"

Epoll::Epoll()
{
}

Epoll::~Epoll()
{
    free(m_events);
    close(m_epfd);
}

bool Epoll::Init()
{
    memset(&m_ev,0x0,sizeof(m_ev));
    m_ev.events = EPOLLIN | EPOLLERR |EPOLLHUP;
    m_ev.data.ptr = NULL;
    m_ev.data.fd = -1;

    m_events = (struct epoll_event*) malloc(MAX_LINK_COUNT * sizeof(struct epoll_event));
    if(NULL == m_events)
    {
        return false;
    }
    m_epfd = epoll_create(MAX_LINK_COUNT);
    if(m_epfd < 0)
    {
        return false;
    }

    return true;
}

void Epoll::Uinit()
{
    for(auto it = m_linkmap.begin(); it!=m_linkmap.end(); it++)
    {
        delete it->second;
        it->second = nullptr;
    }
    m_linkmap.clear();

}

INT32 Epoll::EpollAdd(INT32 connfd)
{
    if(connfd < 0)
    {
        return -1;
    }
    m_linkmap[connfd] = new baselink();
    if(m_linkmap[connfd]->Init(connfd) == false)
    {
        delete m_linkmap[connfd];
        m_linkmap[connfd] = nullptr;
        m_linkmap.erase(connfd);
        return -1;
    }

    m_ev.data.fd = connfd;
    return epoll_ctl(m_epfd, EPOLL_CTL_ADD, connfd, &m_ev);
}

INT32 Epoll::EpollWait(INT32 timeout)
{
    return epoll_wait(m_epfd, m_events, MAX_LINK_COUNT, timeout);
}

struct epoll_event* Epoll::GetEvent(INT32 pos)
{
    if(pos < 0 || pos >= MAX_LINK_COUNT)
    {
        return nullptr;
    }
    return &m_events[pos];
}

baselink* Epoll::GetLinkerByfd(INT32 connfd)
{
    if(m_linkmap.find(connfd) == m_linkmap.end())
    {
        return nullptr;
    }
    return m_linkmap[connfd];
}

void Epoll::EpollRemove(INT32 connfd)
{
    auto t_it = m_linkmap.find(connfd);
    if(t_it != m_linkmap.end())
    {
        if(t_it->second != nullptr)
        {
            delete t_it->second;
            t_it->second = nullptr;
            m_linkmap.erase(t_it);
        }
    }
    //不用手动删epoll中的fd，epoll会自己将close的fd删除掉
}

void Epoll::BroadCast(const MesgInfo& msghead,Message& msg)
{
    MesgHead t_mh ;
    static char s_head_buffer[1024];
    static char s_buffer[1024];
    t_mh.Init(msghead.msgID , msghead.uID, msghead.packLen);
    //std::cout << "msg id " << msghead.msgID <<" uid "<<msghead.uID << std::endl;
    INT32 t_hl = t_mh.encode(s_head_buffer, msg.ByteSizeLong());

    msg.SerializeToArray(s_buffer, msg.ByteSizeLong());

    for(auto it = m_linkmap.begin();it!=m_linkmap.end();it++)
    {
        if(it->second->GetFD() != this->connfd_db_server) {
//            std::cout << "Login server broadcast once" << std::endl;
            it->second->SendData(s_head_buffer, t_hl);
            it->second->SendData(s_buffer,msg.ByteSizeLong());
        }
    }

    return ;
}

//void Epoll::BroadCastToLoginServer(const MesgInfo& msghead,Message& msg)
//{
//    MesgHead t_mh ;
//    static char s_head_buffer[1024];
//    static char s_buffer[1024];
//    t_mh.Init(msghead.msgID , msghead.uID, msghead.packLen);
//    //std::cout << "msg id " << msghead.msgID <<" uid "<<msghead.uID << std::endl;
//    INT32 t_hl = t_mh.encode(s_head_buffer, msg.ByteSizeLong());
//
//    msg.SerializeToArray(s_buffer, msg.ByteSizeLong());
//
//    for(auto it = m_linkmap.begin();it!=m_linkmap.end();it++)
//    {
//        it->second->SendData(s_head_buffer, t_hl);
//        it->second->SendData(s_buffer,msg.ByteSizeLong());
//    }
//
//    return ;
//}

void Epoll::SendMsg(const MesgInfo& msghead,Message& msg,const INT32 connfd)
{
    if(m_linkmap.find(connfd) == m_linkmap.end())
    {
        std::cout << "Can't find connfd to send message. connfd = " << connfd << std::endl;
        return ;
    }
    MesgHead t_mh ;
    static char s_head_buffer[1024];
    static char s_buffer[1024];
    t_mh.Init(msghead.msgID , msghead.uID, msghead.packLen);
    //std::cout << "msg id " << msghead.msgID <<" uid "<<msghead.uID << std::endl;
    INT32 t_hl = t_mh.encode(s_head_buffer, msg.ByteSizeLong());

    msg.SerializeToArray(s_buffer, msg.ByteSizeLong());
    
    m_linkmap[connfd]->SendData(s_head_buffer, t_hl);
    m_linkmap[connfd]->SendData(s_buffer,msg.ByteSizeLong());
    return ;
}

