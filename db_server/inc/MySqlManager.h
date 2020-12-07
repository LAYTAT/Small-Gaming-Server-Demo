#ifndef FORTHWEEK_MYSQLMGR_H
#define FORTHWEEK_MYSQLMGR_H

#include "Singleton.h"
#include "Config.h"
#include "GameSpec.pb.h"
#include <map>
#include <utility>
#include "mysql++/mysql++.h"

class MySqlMgr {
private:
    MySqlMgr();
    ~MySqlMgr();
    DECLARE_SINGLETON(MySqlMgr)
    mysqlpp::Connection* pconn;
    bool updateFun(const std::string& s);//参数只要传入sql语句即可
    bool searchFun(const std::string& s,mysqlpp::StoreQueryResult& res);
    bool insertFun(const std::string& s);//参数只要传入sql语句即可
    bool existFun(const std::string& s,bool& res);
    bool UpdateOneCol(int id,const std::string& ColName,int num);
    bool UpdateOneCol(int id,const std::string& ColName,const std::string str);


public:
    bool Init();
    void Uinit();
    bool GetPlayerInfo(int id,GameSpec::Players* res);
    bool UpdatePlayerInfo(GameSpec::Players* input);
    bool CreateNewPlayer(GameSpec::Players* input);
    int GetPlayerId(const std::string& input);
    bool UpdateName(int id,const std::string& newName);
    bool GetAll(std::map<int,std::pair<int,std::string>>& res/*std::vector<int>& res_id, std::vector<int>& res_exp*/);
    bool UpdateBagInfo(int id,const std::string& info);
    bool UpdateExp(int id,int exp);
    bool UpdateRank(int id,int rank);
    bool GetAllUserNameAndPwd(std::map<std::string,std::string>& res);


};

#endif //FORTHWEEK_MYSQLMGR_H
