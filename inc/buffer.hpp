//
// Created by laylei on 2020/12/3.
//

#ifndef MY_SERVER_BUFFER_HPP
#define MY_SERVER_BUFFER_HPP

#include "Config.h"

class buffer
{
public:
    buffer();
    ~buffer();

    bool Init(INT32 sockid);
    void Uinit();

    INT32 CheckAndReset();   //cheak cache, and get buffer lens which can be used
    INT32 RecvData();

    char * GetHead(){ return &m_pbuffer[m_phead]; }
    char * GetTail(){ return &m_pbuffer[m_ptail]; }
    INT32 GetSize(){ return m_ptail-m_phead;}  //Get saved datas size
    char * GetBuffer(INT32& len){ len = m_ptail-m_phead; m_ptail = m_phead; return &m_pbuffer[m_phead]; } //Get all datas in buffer
    char * GetPack(INT32 len){ INT32 tmp = m_phead; m_phead += len; return &m_pbuffer[tmp]; }  //Get one Package

private:
    INT32 m_psocketfd;
    INT32 m_phead;
    INT32 m_ptail;
    char m_pbuffer[BUFFER_SIZE+1];
};

buffer::buffer()
{
}

buffer::~buffer()
{
}

bool buffer::Init(INT32 sockid)
{
    m_phead = 0;
    m_ptail = 0;
    m_psocketfd = sockid;

    memset(m_pbuffer, 0x0, sizeof(char)*(BUFFER_SIZE+1));
    return true;
}

void buffer::Uinit()
{

}

INT32 buffer::CheckAndReset()
{
    if (m_ptail == BUFFER_SIZE)
    {
        if (m_phead == 0)
        {
            return 0;
        }
        else
        {
            memmove(&m_pbuffer[0], &m_pbuffer[m_phead], m_ptail - m_phead );
            m_phead = 0;
            m_ptail = m_ptail - m_phead;
        }
    }
    return BUFFER_SIZE-m_ptail;
}

INT32 buffer::RecvData()
{
    INT32 len = CheckAndReset();
    if (len == 0)
    {
        return 0;
    }
    INT32 ret = recv(m_psocketfd, this->GetTail(), len, 0);
    //std::cout << "received data's len = " << ret << "   from sockfd = "<< m_psocketfd << std::endl;

    if( ret >0 )
    {
        m_ptail += ret;
    }
    else
    {
        if((ret<0) &&(errno == EAGAIN||errno == EWOULDBLOCK||errno == EINTR)) //这几种错误码，认为连接是正常的，继续接收
        {
            std::cout << "ret< 0 || errno , ret = "<< ret<<"   errno = "<<errno<<std::endl;
            return 0;
        }

        std::cout << "ret == 0"<< ret<<"   errno = "<<errno<<std::endl;
        //CloseSocket();
        return -1;
    }
    return ret;
}



#endif //MY_SERVER_BUFFER_HPP
