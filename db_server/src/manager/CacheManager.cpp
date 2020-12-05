//
// Created by laylei on 2020/12/2.
//

#include "CacheManager.h"

#include <iostream>

#define timeout 200

INSTANCE_SINGLETON(CacheManager)

using namespace sw::redis;

CacheManager::CacheManager(){
    std::cout << "CacheManager Constructed !" << std::endl;
}

CacheManager::~CacheManager(){
    delete redisConnection;
    redisConnection = nullptr;
    std::cout << "CacheManager Destroyed ~" << std::endl;
}

bool CacheManager::Init()
{
    ConnectRedis("127.0.0.1",6379,"123456",Players);
    std::map<int32_t,std::pair<int32_t,std::string> > t_map;
    MySqlMgr::Instance()->GetAll(t_map);
    reinit(t_map);// 添加玩家id到布隆过滤器，添加用户的账号密码到redis
    return  true;
}

// 指定 host port password and database
void CacheManager::ConnectRedis(std::string host = "127.0.0.1", int port = 6379, std::string password = "123456", Database database = Players) {
    // Connect with redis
    try {
        // 设置密码等信息
        ConnectionOptions connection_options;
        connection_options.host = host;  // Required.
        connection_options.port = port; // Optional. The default port is 6379.
        connection_options.password = password;   // Optional. No password by default.
        //connection_options.socket_timeout = std::chrono::milliseconds(timeout); // 超时时间
        connection_options.db = database;  // Optional. Use the 0th database by default.

        // Create an Redis object, which is movable but NOT copyable.
        this->redisConnection = new Redis(connection_options);

        // 初始化bloom过滤器
        BloomFilterInit();
    } catch (const Error &e) {
        std::cout << "我是金六福，我写的redis崩了" << "。\n 错误的原因: "<< e.what() <<std::endl;
    }
}

void CacheManager::ConnectRedis(std::string host = "127.0.0.1", int port = 6379, std::string password = "123456") {
    // Connect with redis
    try {
        // 设置密码
        ConnectionOptions connection_options;
        connection_options.host = host;  // Required.
        connection_options.port = port; // Optional. The default port is 6379.
        connection_options.password = password;   // Optional. No password by default.
        // connection_options.socket_timeout = std::chrono::milliseconds(timeout); // 超时时间
        // connection_options.db = 1;  // Optional. Use the 0th database by default.

        // Create an Redis object, which is movable but NOT copyable.
        this->redisConnection = new Redis(connection_options);
        // 初始化bloom过滤器
        BloomFilterInit();
    } catch (const Error &e) {
        std::cout << "我是金六福，我写的redis崩了" << "。\n 错因: "<< e.what() <<std::endl;
    }
}

// example for connect_string: tcp://127.0.0.1:6379
void CacheManager::ConnectRedis(std::string connect_string = "tcp://127.0.0.1:6379") {
    // Connect with redis
    try {
        // Create an Redis object, which is movable but NOT copyable.
        this->redisConnection = new Redis(connect_string);
        // 初始化bloom过滤器
        BloomFilterInit();
    } catch (const Error &e) {
        std::cout << "我是金六福，我写的redis崩了" << "。\n 错因: "<< e.what() <<std::endl;
    }
}

sw::redis::Redis* CacheManager::getRedisConnection() {
    return this->redisConnection;
}

void CacheManager::Uinit()
{
    std::cout << "CacheManager Uninitialized!" << std::endl;
    return;
}

std::vector<OptionalString> CacheManager::getPlayerInfos(int playerId){
    if(containsPlayerId(playerId)) {
        // Get value only.
        // NOTE: since field might NOT exist, so we need to parse it to OptionalString.
        std::vector<OptionalString> vals;
        redisConnection->hmget("player:" + std::to_string(playerId), {"exp", "rank", "name"}, std::back_inserter(vals));
        for (const auto &val : vals) {
            if (val)
                std::cout <<  *val << std::endl;
            else
                std::cout << "field not exist" << std::endl;
        }
        return vals;
    } else {
        // TODO: 错误处理
        return {{}};
    }
}

std::string CacheManager::getPlayerInfo(int playerId, std::string which_info) {
    if(containsPlayerId(playerId)) {
        // Get value only.
        // NOTE: since field might NOT exist, so we need to parse it to OptionalSt rin
        std::vector<OptionalString> vals;
        redisConnection->hmget("player:" + std::to_string(playerId), {which_info}, std::back_inserter(vals));
        return vals[0] ? * vals[0]: "player:" + std::to_string(playerId) + " does not have the field of：" + which_info;
    } else {
        return std::string();
    }

}

void CacheManager::setPlayerInfo(int id, std::string name, int exp) {
    // std::unordered_map<std::string, std::string> to Redis HASH.
    std::unordered_map<std::string, std::string> m = {
            {"id", std::to_string(id) },
            {"name", name},
            {"exp", std::to_string(exp)},
    };
    updateRankWithExp(id, exp, UPDATE);
    redisConnection->hmset("player:" + std::to_string(id), m.begin(), m.end());
}

void CacheManager::newPlayer(int playerId, std::string name, int exp) {
    // std::unordered_map<std::string, std::string> to Redis HASH.
    std::unordered_map<std::string, std::string> m = {
            {"id", std::to_string(playerId) },
            {"name", name},
            {"exp", std::to_string(exp)},
    };
    // 更新排名
    updateRankWithExp(playerId, exp, UPDATE);
    // 添加到布隆过滤器
    bloomFilter.insert(playerId);

    redisConnection->hmset("player:" + std::to_string(playerId), m.begin(), m.end());
}

void CacheManager::updateRankWithExp(int playerId, int exp, RankOperationEnum operationEnum) {
    if(operationEnum == UPDATE) {
        redisConnection->zadd("playerRank", "player:"+std::to_string(playerId), exp);
    } else if (operationEnum == INCRBY) {
        redisConnection->zincrby("playerRank", exp, "player:"+std::to_string(playerId));
    }
}



std::vector<std::string> CacheManager::getPlayerNamesByExpRange(float minExp, float maxExp) {
    // Only get member names:
    std::vector<std::string> without_score;
    redisConnection->zrangebyscore("playerRank",
                                   BoundedInterval<double>(minExp, maxExp,BoundType::CLOSED),   // [minExp, maxExp]
                                   std::back_inserter(without_score));
    return without_score;
}

std::unordered_map<std::string, double> CacheManager::getPlayerNamesAndScoresByExpRange(float minExp, float maxExp){
    // Get both member names and scores:
    // pass an inserter of std::unordered_map<std::string, double> as output parameter.
    std::unordered_map<std::string, double> with_score;
    redisConnection->zrangebyscore("playerRank",
                                   BoundedInterval<double>(minExp, maxExp, BoundType::CLOSED),    // [minExp, maxExp]
                                   std::inserter(with_score, with_score.end()));
    return with_score;
}

int CacheManager::getPlayerRank(int playerId) {
    if(containsPlayerId(playerId)) {
        auto oll = redisConnection->zrank("playerRank", "player:"+std::to_string(playerId));
        if (oll) {
            std::cout << "rank is " << *oll << std::endl;
            return *oll;
        } else {
            std::cerr << "member doesn't exist" << std::endl;
            return 0;
        }
    } else {
        return 0;
    }
}

std::unordered_map<std::string, double> CacheManager::getTopNPlayer(int top) {
    std::unordered_map<std::string, double> res_with_score;
    redisConnection->zrevrange("playerRank", 0, top, std::inserter(res_with_score, res_with_score.end()));
    return res_with_score;
}

//TODO: 测试这个接口
bool CacheManager::getBagInfoFromPlayer(int playerId, GameSpec::BagInfo * baginfo) {
    if(containsPlayerId(playerId)) {
        // Get value only.
        // NOTE: since field might NOT exist, so we need to parse it to OptionalSt rin
        std::vector<OptionalString> vals;
        redisConnection->hmget("player:" + std::to_string(playerId), {"baginfo"}, std::back_inserter(vals));
        if (!vals[0]) { // 在redis中找不到数据
            std::cout << "baginfo not found" << std::endl;
            return false;
        }
        GameSpec::BagInfo ret_bagInfo;

        if (ret_bagInfo.ParseFromString(*vals[0])){
            std::cout << "baginfo found and unserialized" << std::endl;
            baginfo =  &ret_bagInfo;
            return true;
        } else {
            std::cout << "baginfo not unserialized, failed .." << std::endl;
            return false;
        }
    } else {
        return false;
    }
}

//TODO: 测试这个接口
void CacheManager::setBagInfoOfPlayer(int playerId, GameSpec::BagInfo * bagInfo) {
    std::unordered_map<std::string, std::string> m = {
            {"baginfo", bagInfo->SerializeAsString() }
    };
    redisConnection->hmset("player:" + std::to_string(playerId), m.begin(), m.end());
}

void CacheManager::BloomFilterInit(){
    bloom_parameters parameters;
    parameters.projected_element_count = PROJECTED_ELEMENT_COUNT;
    parameters.false_positive_probability = FALSE_POSITIVE_PROBOBILITY;
    parameters.random_seed = RANDOM_SEED;
    if (!parameters)
    {
        std::cout << "Error - Invalid set of bloom filter parameters!" << std::endl;
    }

    bloom_filter filter(parameters);
    this->bloomFilter = filter;
    std::vector<int>* playerIdList = new std::vector<int>({1,2,3,4,5,6,7,8,9}); // TODO: 改为真实存在玩家账号, 接上mysql的获取所有玩家id的接口

    // Insert into Bloom Filter
    {
        // Insert some strings
        for (std::size_t i = 0; i < (sizeof(playerIdList) / sizeof(std::string)); ++i)
        {
            filter.insert(playerIdList[i]);
        }
    }
    delete playerIdList;
}

bool CacheManager::containsPlayerId(int playerId) {
    if(this->bloomFilter.contains(playerId)) {
        return true;
    } else {
        std::cout<<"Search is blocked by bloom filter."<<std::endl;
    }
    return true;
}

// TODO: 接入mysql
// 这里不仅重新写入了玩家的信息到redis，还插入了新的布隆过滤器列表
void CacheManager::reinit(std::map<int,std::pair<int, std::string>> &res) {
    std::vector<int> playerId_list;
    for(auto &key : res) {
        newPlayer(key.first, key.second.second, key.second.first);
        playerId_list.push_back(key.first);
    }
    this->bloomFilter.insert(playerId_list);
}
