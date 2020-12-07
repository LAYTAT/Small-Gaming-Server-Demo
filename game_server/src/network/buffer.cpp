#include "buffer.h"

buffer::buffer()
{
}

buffer::~buffer()
{
}

bool buffer::Init()
{
    m_phead = 0;
    m_ptail = 0;
    m_fulled = false;

    memset(m_pbuffer, 0x0, sizeof(char)*(BUFFER_SIZE+1));
    memset(m_ptmpbuffer, 0x0, sizeof(char)*(1024+1));
    return true;
}

void buffer::Uinit()
{
    
}

INT32  buffer::GetWritableLens()
{
    if( m_fulled )
    {
        return 0;
    }
    if( m_phead <= m_ptail )
    {
         return (INT32)(BUFFER_SIZE)-m_ptail;
    }
    return m_phead - m_ptail; 
}

INT32 buffer::GetReadableLens()
{
    if( m_fulled )
    {
        return (INT32)(BUFFER_SIZE);
    }
    if( m_phead <= m_ptail )
    {
        return m_ptail - m_phead;
    }
    return (INT32)(BUFFER_SIZE) - ( m_phead - m_ptail ) ;
}

bool buffer::MoveHead(INT32 offset)
{
    if(offset == 0)
    {
        return true;
    }
    if(offset > GetReadableLens())
    {
        std::cout << "MoveHead'lens > readablelens"<<std::endl;
        return false; //移动的距离大于能读的距离，closesocket
    }
    m_phead = (m_phead+offset)%(INT32)(BUFFER_SIZE);
    if(m_fulled && m_phead != m_ptail)
    {
        m_fulled = false;
    }
    return true;
}

bool buffer::MoveTail(INT32 offset)
{
    if(offset == 0)
    {
        return true;
    }
    if(offset > GetWritableLens())
    {
        std::cout << "MoveTail'lens > writablelens"<<std::endl;
        return false; //移动的距离大于能写的距离，closesocket
    }
    m_ptail = (m_ptail+offset)%(INT32)(BUFFER_SIZE);
    if(!m_fulled && m_ptail == m_phead)
    {
        m_fulled = true;
    }
    return true;
}

char * buffer::GetPack(INT32 len)
{
    if( len + m_phead > (INT32)(BUFFER_SIZE) )
    {
        memcpy(&m_ptmpbuffer[0], &m_pbuffer[m_phead], BUFFER_SIZE - m_phead);
        memcpy(&m_ptmpbuffer[BUFFER_SIZE - m_phead], &m_pbuffer[0], len - ( BUFFER_SIZE - m_phead) );
        return &m_ptmpbuffer[0];
    }

    return &m_pbuffer[m_phead];
}
