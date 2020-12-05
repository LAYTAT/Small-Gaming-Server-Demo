#ifndef _MESGHEAD_H_
#define _MESGHEAD_H_

#include "Config.h"

struct MesgInfo{
    INT32 msgID;
    INT32 uID;
    INT32 packLen;
};

class MesgHead
{
public:
    MesgHead();
    ~MesgHead();
    
    bool Init(INT32 msg, INT32 uid, INT32 lens);
    void Uinit();

    INT32 GetMsgHeadSize();  // 获得一个包头的长度（固定的）

    INT32 encode(char* buffer, INT32 len);
    void decode(char * buffer);
    
public:
    MesgInfo m_msginfo;  // 为什么是public：由于本来msgheader就对Socket是透明的，没必要搞成private

};

#endif
