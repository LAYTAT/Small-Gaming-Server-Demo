
#ifndef GAME_SERVER_CACHE_DATABASEENUM_H
#define GAME_SERVER_CACHE_DATABASEENUM_H


enum Database {
    Players = 1,            // redis-plus-plus starts specifying data with 1 instead of 0
//    BagInfo = 2,          // 从 player-id 映射到 bagItem，equipItem以及currencyItem的表
    BagItems = 2,           // bagitem-id       -> bagitem infos
    EquipItems = 3,         // equipitem-id     -> equipitem info
    CurrencyItems = 4,      // currencuItem-id  -> curencyItem info
    Rank = 5,               // 用于存储玩家的rank
    Auth = 6 ,              // 用于存储玩家的基本信息
};

enum RankOperationEnum {
    INCRBY,
    UPDATE
};

#endif //GAME_SERVER_CACHE_DATABASEENUM_H
