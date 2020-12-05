#ifndef _BASELINK_H_
#define _BASELINK_H_

#include "buffer.h"
#include "MesgHead.h"

class baselink
{
public:
    baselink();
    ~baselink();

    bool Init(INT32 sock);
    void Uinit();
    void SetFD(INT32 sock){ m_socketfd = sock; }   //设置fd

    inline INT32 GetFD(){ return m_socketfd; }  
    INT32 OpenServer(INT32 port, char* IP = NULL);  
    INT32 OpenClient(INT32 port);
    INT32 ConnectServer();   //连接到服务器，一般给客户端使用
    INT32 AcceptSocket();   //接受连接，一般给服务器使用
    
    INT32 RecvData();  //收数据
    INT32 SendData(char *data, INT32 len); //发数据
    INT32 GetPackLens();    // -1代表没有完整的包，否则返回一个完整的包的长度
    const MesgInfo GetMsginfo(){return m_msghead->m_msginfo;} // 获得包头
    char * GetPack(INT32 len);  //Get one Package, and len is the length of pack'body

    INT32 set_non_block(INT32 fd);

    void CloseSocket();

private:
    INT32 m_socketfd;
    buffer* m_buffer; 
    MesgHead* m_msghead;
};

#endif
