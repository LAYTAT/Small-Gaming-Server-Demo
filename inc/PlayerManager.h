//
// Created by laylei on 2020/12/5.
//

#ifndef MY_SERVER_PLAYERMANAGER_H
#define MY_SERVER_PLAYERMANAGER_H

#include "Config.h"
#include "Player.h"
#include "Singleton.h"

class PlayerMgr
{
private:
    PlayerMgr();
    ~PlayerMgr();
DECLARE_SINGLETON(PlayerMgr)

public:
    bool Init();
    void Uinit();
    bool RegisterPlayer(INT32 uid);
    Player* GetPlayerByID(INT32 uid){ return m_players[uid]; }


private:
    //Players Map: uid -> Player*
    std::map<INT32, Player*> m_players;



};


#endif //MY_SERVER_PLAYERMANAGER_H
