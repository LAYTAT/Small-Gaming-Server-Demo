#include "MesgHead.h"
#include <stdint.h>

MesgHead::MesgHead()
{

}

MesgHead::~MesgHead()
{

}

bool MesgHead::Init(INT32 msg, INT32 uid, INT32 lens)
{
    if (msg < 0 || uid < 0 || lens < 0)
    {
        std::cout << "MesgHead init failed " << std::endl;
        return false;
    }
    m_msginfo.msgID = msg;
    m_msginfo.uID = uid;
    m_msginfo.packLen = lens;

    return true;
}

void MesgHead::Uinit()
{
    m_msginfo.msgID = 0;
    m_msginfo.uID = 0;
    m_msginfo.packLen = 0;
}

INT32 MesgHead::encode(char* buffer, INT32 len)
{
    
    uint32_t tmp = htonl(m_msginfo.msgID);
    memcpy(buffer, &tmp, sizeof(INT32));    
    buffer += sizeof(INT32);

    tmp = htonl(m_msginfo.uID);
    memcpy(buffer, &tmp, sizeof(INT32));    
    buffer += sizeof(INT32);

    m_msginfo.packLen = len;
    tmp = htonl(m_msginfo.packLen);
    memcpy(buffer, &tmp, sizeof(INT32)); 
    buffer += sizeof(INT32);
    return 3*sizeof(INT32);
}
void MesgHead::decode(char* buffer)
{
    INT32 tmp = 0;
	memcpy(&tmp, buffer, sizeof(INT32));
    m_msginfo.msgID = ntohl(tmp);
    buffer += sizeof(INT32);

    memcpy(&tmp, buffer, sizeof(INT32));
    m_msginfo.uID = ntohl(tmp);
    buffer += sizeof(INT32);

    memcpy(&tmp, buffer, sizeof(INT32));
    m_msginfo.packLen = ntohl(tmp);
    buffer += sizeof(INT32);
}

INT32 MesgHead::GetMsgHeadSize()
{
    return 3*sizeof(INT32);
}
