//
// Created by laylei on 2020/12/5.
//

#include "Player.h"

Player::Player(INT32 uid)
{
    m_uid = uid;
    m_state = 0;
    m_hp = 100;
}

Player::~Player()
{

}

INT32 Player::test_func(MesgInfo &stHead, const char *body, const INT32 len)
{
    std::cout << "hello world £¡" << std::endl;
    return 0;
}

INT32 Player::GetID()
{
    return m_uid;
}
