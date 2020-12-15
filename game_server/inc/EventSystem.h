//
// Created by laylei on 2020/12/5.
//

#ifndef MY_SERVER_EVENTSYSTEM_H
#define MY_SERVER_EVENTSYSTEM_H

#include "Config.h"
#include "Singleton.h"
#include "MsgHandler.h"
#include <functional>
#include "BagSystem.h"


class EventSystem
{
private:
    EventSystem();
    ~EventSystem();
DECLARE_SINGLETON(EventSystem);
public:
    bool Init();
    void Uinit();

    MsgHandler<EventSystem>*  GetMsgHandler() { return m_msgHandler; }

    INT32 PlayerReqItems(const MesgInfo &stHead, const char *body, const INT32 len,const INT32 connfd);

    // Íæ¼ÒÒÆ¶¯
    INT32 PlayerMove(const MesgInfo &stHead, const char *body, const INT32 len,const INT32 connfd);


private:
    MsgHandler<EventSystem>* m_msgHandler;

    BagSystem m_bagSystem;

    INT32 PlayerRegister(const MesgInfo &stHead, const char *body, const INT32 len, const INT32 connfd);
};


#endif //MY_SERVER_EVENTSYSTEM_H
