//
// Created by laylei on 2020/12/5.
//

#ifndef MY_SERVER_EVENTSYSTEM_H
#define MY_SERVER_EVENTSYSTEM_H

#include "Config.h"
#include "Singleton.h"
#include "MsgHandler.h"
#include "EntityMgr.h"
#include <functional>
#include "BagSystem.h"
#include "unordered_map"

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

    // ≥È¿ÎµΩgame server
    INT32 PlayerRegister(const MesgInfo &stHead, const char *body, const INT32 len,const INT32 connfd);
    INT32 PlayerMove(const MesgInfo &stHead, const char *body, const INT32 len,const INT32 connfd);
    INT32 PlayerAddItem(const MesgInfo &stHead, const char *body,const INT32 len,const INT32 connfd);
    INT32 PlayerRemoveItem(const MesgInfo &stHead, const char *body,const INT32 len,const INT32 connfd);
    INT32 PlayerLogin(const MesgInfo &stHead, const char *body,const INT32 len,const INT32 connfd);
    INT32 GetRankList(const MesgInfo &stHead, const char *body,const INT32 len,const INT32 connfd);
    INT32 PlayerLoginPassed(const MesgInfo &stHead, const char *body,const INT32 len,const INT32 connfd);
    INT32 PlayerLoginFailed(const MesgInfo &stHead, const char *body,const INT32 len,const INT32 connfd);
private:
    MsgHandler<EventSystem>* m_msgHandler;
    std::unordered_map<INT32,INT32> player_id_to_fd_map;
    BagSystem m_bagSystem;
};


#endif //MY_SERVER_EVENTSYSTEM_H
