//
// Created by laylei on 2020/12/2.
//

#ifndef GAME_SERVER_CACHE_CACHEMANAGER_H
#define GAME_SERVER_CACHE_CACHEMANAGER_H

#include "Singleton.h"
#include <sw/redis++/redis++.h>
#include "CacheEnum.h"
#include "ItemBase.h"
#include "ItemList.h"
#include "GameSpec.pb.h"
#include "bloom_filter.hpp"
#include "Config.h"
#include "MySqlManager.h"

// 新建布隆过滤器的配置参数 ==========================================
// 大致有多少东西能在数据库被查到
#define PROJECTED_ELEMENT_COUNT 1000
// 需要的错误率，越低当然越好，但是会消耗cpu，因为会增加很多哈希计算，适当即可
#define FALSE_POSITIVE_PROBOBILITY 0.0001
// 随机种子，随意
#define RANDOM_SEED 0xA5A5A5A5
// 新建布隆过滤器的配置参数 ==========================================


class CacheManager {
private:
    CacheManager();
    ~CacheManager();
    sw::redis::Redis* redisConnection;
    // 布隆过滤器 TODO: 完善bloom过滤器
    bloom_filter bloomFilter;
DECLARE_SINGLETON(CacheManager);
public:
    bool Init();
    void Uinit();
    // 三个redis连接的新建重载方法
    void ConnectRedis(std::string host , int port, std::string password);
    void ConnectRedis(std::string host , int port , std::string password, Database database);
    void ConnectRedis(std::string connect_string);
    sw::redis::Redis* getRedisConnection();
    // 布隆过滤器的新建方法
    void BloomFilterInit();
    bool containsPlayerId(int);

    void reinit(std::map<int,std::pair<int,std::string>> &);

    // TODO: 添加到mysql的连接

    /* 获得玩家的baginfo */
    // 如果成功获取则返回true，如果没能成功获取则返回false
    bool getBagInfoFromPlayer(int playerId, GameSpec::BagInfo * baginfo);
    void setBagInfoOfPlayer(int playerId, GameSpec::BagInfo * bagInfo);

    /***  BagItem相关存取接口  ***/
    void addItemToPlayer(int playerId, int ItemId, int pos, ItemBase* item);
    ItemBase* getItem(int playerId, int itemId, int pos);
    std::string getItemInfo(int playerId, int ItemId, int pos, std::string which_info);

    /***  Player相关存取接口  ***/
    // 新建玩家到数据库, 新的玩家务必添加，否则会被布隆过滤器过滤掉
    void newPlayer(int playerId, std::string name, int exp);
    // 获取玩家的所有基本信息
    std::vector<sw::redis::OptionalString> getPlayerInfos(int playerId);
    // 设置玩家的Info，其实和newPlayer一致
    void setPlayerInfo(int id, std::string name, int exp);
    // which info 多用于声明要找的字段
    std::string getPlayerInfo(int playerId, std::string which_info);
    // 玩家排名相关的接口
    // 使用经验值更新玩家的排名
    void updateRankWithExp(int playerId, int exp, RankOperationEnum operationEnum = UPDATE);
    // 获得特定分段的玩家id
    std::vector<std::string> getPlayerNamesByExpRange(float minExp, float maxExp);
    std::unordered_map<std::string, double> getPlayerNamesAndScoresByExpRange(float minExp, float maxExp);

    // 获得某位玩家的排名
    int getPlayerRank(int playerId);

    // 获得排名前n的玩家们
    std::unordered_map<std::string, double> getTopNPlayer(int top);
};
#endif //GAME_SERVER_CACHE_CACHEMANAGER_H

