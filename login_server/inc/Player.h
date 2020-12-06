//
// Created by laylei on 2020/12/5.
//

#ifndef MY_SERVER_PLAYER_H
#define MY_SERVER_PLAYER_H

#include "Config.h"
#include "MesgHead.h"

class Player
{
public:
    Player(INT32 uid);
    ~Player();

    INT32 GetID();
    INT32 test_func(MesgInfo &stHead, const char *body, const INT32 len);


private:
    INT32 m_uid;
    INT32 m_state;
    INT32 m_hp;
};


#endif //MY_SERVER_PLAYER_H
