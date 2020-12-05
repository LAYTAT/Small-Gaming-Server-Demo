//
// Created by laylei on 2020/12/5.
//

#ifndef MY_SERVER_MSGHANDLER_H
#define MY_SERVER_MSGHANDLER_H

#include "Config.h"
#include "MesgHead.h"

template<class T>
class MsgHandler {
public:
    typedef INT32 (T::*funHandle) (const MesgInfo& stHead, const char* body, const INT32 len,const INT32 connfd);
public:
    void RegisterMsg(int32_t MessageID, funHandle func){
        if (m_mapRequest.find(MessageID) != m_mapRequest.end()) {
            printf("[MsgHandler::%s] conflict client msg id(id:%d)\n", __FUNCTION__, MessageID);
            exit(0);
        }
        m_mapRequest[MessageID] = func;
    }

    funHandle GetMsgFunc(int32_t MessageID){
        auto it = m_mapRequest.find(MessageID);
        if ( it != m_mapRequest.end() ) {
            return it->second;
        }
        printf("[MsgHandler::%s] iMessageID=(%d | 0x%x) is not register\n", __FUNCTION__, MessageID,MessageID);
        return NULL;
    }

    void Uinit(){
        m_mapRequest.clear();
    }
private:
    std::map<int32_t, funHandle> m_mapRequest;
};


#endif //MY_SERVER_MSGHANDLER_H
