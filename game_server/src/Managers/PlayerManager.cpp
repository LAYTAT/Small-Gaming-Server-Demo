//
// Created by laylei on 2020/12/5.
//

#include "PlayerManager.h"

INSTANCE_SINGLETON(PlayerMgr)

PlayerMgr::PlayerMgr()
{
}

PlayerMgr::~PlayerMgr()
{

}

bool PlayerMgr::Init()
{
    std::cout << "PlayerMgr Init Success!" << std::endl;
    m_players.clear();
    return true;
}

void PlayerMgr::Uinit()
{
    for(auto it = m_players.begin(); it != m_players.end(); it++)
    {
        delete it->second;
        it->second = nullptr;
    }
}

bool PlayerMgr::RegisterPlayer(INT32 uid)
{
    if (m_players.find(uid) != m_players.end())
    {
        std::cout << "Player's uid has already exist. uid = " << uid << std::endl;
        return false;
    }
    Player* t_per = new Player(uid);
    m_players[uid] = t_per;
    std::cout << "A player register in server, uid = " << uid << std::endl;

    return true;
}
