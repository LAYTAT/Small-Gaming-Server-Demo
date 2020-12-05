#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "Config.h"

class buffer
{
public:
    buffer();
    ~buffer();
    
    bool Init();
    void Uinit();

    INT32 RecvData();  // 收数据
    
    char * GetHead(){ return &m_pbuffer[m_phead]; }  // 获得第一个有数据的地址
    char * GetTail(){ return &m_pbuffer[m_ptail]; }  // 获得第一个可以写的位置
    bool MoveHead(INT32 offset);  // 移动读指针
    bool MoveTail(INT32 offset);  // 移动写指针
    char * GetPack(INT32 len);  //获得整个包长的包，看一下是否是缓冲区边缘的包，是的话就用一个新的小缓冲区返回

    INT32 GetWritableLens();  // 获取这次可写的空间大小，不是剩余空间的大小
    INT32 GetReadableLens();  // 获取目前buffer所存数据的大小
    

private:
    INT32 m_phead;   // 读指针
    INT32 m_ptail;   // 写指针
    char m_pbuffer[BUFFER_SIZE+1];  //主要buffer，收包
    char m_ptmpbuffer[1024+1];   //处理边缘数据的临时buffer
    bool m_fulled;  //判断缓冲区是不是满了
};

#endif
