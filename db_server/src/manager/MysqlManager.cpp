//
// Created by laylei on 2020/12/2.
//
#include "MySqlManager.h"
using namespace std;
using namespace mysqlpp;

INSTANCE_SINGLETON(MySqlMgr)

MySqlMgr::MySqlMgr() //1.私有 单例模式
{
}

MySqlMgr::~MySqlMgr() //2.私有 单例模式
{
}

bool MySqlMgr::Init() // 1.公有 初始化数据库连接
{
    try {
        pconn = new Connection(use_exceptions);
        auto ret = pconn->connect(DATABASE_NAME, DATABASE_IP, DATABASE_USERNAME, DATABASE_PWD);
    }
    catch (BadQuery er)
    { // handle any connection or
        // query errors that may come up
        cerr << "Error: " << er.what() << endl;
        return false;
    }
    catch (const BadConversion& er)
    {
        // Handle bad conversions
        cerr << "Conversion error: " << er.what() << endl <<
             "\tretrieved data size: " << er.retrieved <<
             ", actual size: " << er.actual_size << endl;
        return false;
    }
    catch (const Exception& er)
    {
        // Catch-all for any other MySQL++ exceptions
        cerr << "Error: " << er.what() << endl;
        return false;
    }

    return true;

}

bool MySqlMgr::insertFun(const string& s)//3.私有成员函数 ： 增
{
    try {
        Query pQuery = pconn ->query();
        cout << "Inserting test" << endl;
        pQuery << s.c_str();
        SimpleResult res = pQuery.execute();
    }catch (const Exception& er)
    {
        cout << "insert failed(" <<s << ")" <<"ERROR:" << er.what()<<endl;
        return false;
    }
    return true;
}

bool MySqlMgr::searchFun(const string& s,StoreQueryResult& res) //4.私有成员函数 ： 查
{
    try {
        Query pQuery = pconn ->query();
        pQuery << s.c_str();
        res = pQuery.store();
    }catch (const Exception& er)
    {
        cout << "searchFun1 failed(" <<s << ")" <<"ERROR:" << er.what()<<endl;
        return false;
    }
    return true;
}

bool MySqlMgr::updateFun(const string& s) //5.私有成员函数 ：改
{
    try {
        Query pQuery = pconn->query();
        pQuery << s.c_str();
        if (pQuery.exec())
        {
            cout << "updated success!" << endl;
        }
    }
    catch (const Exception& er)
    {
        cout << "updateFun failed(" <<s << ")" <<"ERROR:" << er.what() <<endl;
        return false;
    }
    return true;
}

bool MySqlMgr::existFun(const std::string& s,bool& res) //6. 私有成员函数 ：查询是否存在
{
    try {
        Query pQuery = pconn ->query();
        pQuery << s.c_str();
        StoreQueryResult ares = pQuery.store();
        if(ares.num_rows())
            res = true;
    }catch (const Exception& er)
    {
        cout << "searchFun1 failed(" <<s << ")" <<"ERROR:" << er.what()<<endl;
        return false;
    }
    return true;
}

bool MySqlMgr::UpdateOneCol(int id,const std::string& ColName,int num) //7. 私有的成员函数 ：更改一个int属性的字段
{
    if(ColName.c_str() !="rank" && ColName.c_str()!= "exp")
        return false;
    string a ="update players set " +ColName + "=" +std::to_string(num);
    return updateFun(a);
}

bool MySqlMgr::UpdateOneCol(int id,const std::string& ColName,const std::string str) //8.私有的成员函数 ： 更改一个int属性的字段 重载版本
{
    if(ColName.c_str() !="playerName" && ColName.c_str() !="bagInfo")
        return false;
    string a ="update players set " +ColName + "= '" +str + "'";
    return updateFun(a);
}


bool MySqlMgr::GetPlayerInfo(int id,GameSpec::Players* res)  //2. 公有  根据玩家ID 取回玩家的所有信息
{
    if(res == nullptr)
        return  false;
    mysqlpp::StoreQueryResult ares;
    std::string a ="select * from players where id=" +std::to_string(id);
    int ret = searchFun(a,ares);
    if(ret)
    {
        int idTmp =ares[0]["id"];
        res->set_id(idTmp);

        mysqlpp::String ppTmp =ares[0]["playerName"];
        std::string playerNameTmp =std::string(ppTmp.data(),ppTmp.length());
        res->set_name(playerNameTmp.c_str());

        int rankTmp=ares[0]["rank"];
        res->set_rank(rankTmp);

        int expTmp=ares[0]["exp"];
        res->set_exp(expTmp);

        mysqlpp::String infoppTmp =ares[0]["bagInfo"];
        if(infoppTmp.length() != 0)
        {
            std::string blobTmp =std::string(infoppTmp.data(),infoppTmp.length());

            GameSpec::BagInfo* bagInfo = res->mutable_bags();

            bagInfo->ParseFromString(blobTmp);

            //res->mutable_bags()->ParseFromString(blobTmp);

        }
    }

    return ret;
}

bool MySqlMgr::UpdatePlayerInfo(GameSpec::Players* input)
{
    if(input == nullptr)
        return false;

    //std::string a ="update players set ";
    int id =input->id();
    int rankUp =input->rank();
    int expUp =input->exp();

    std::string bagInfoUp;
    GameSpec::BagInfo bagUp =input->bags();
    bagUp.SerializeToString(&bagInfoUp);
    /*
    if(input_field()->id() != 0)
    {
        id=input->id();
    } else
    {
        return false;
    }

    bool ret1 = true;
    if(input->has_rank())
    {
        int rankUp =input->rank();
        ret1 =UpdateOneCol(id,"rank",rankUp);
    }

    bool ret2 = true;
    if(input->has_exp())
    {
        int expUp =input->exp();
        ret2 =UpdateOneCol(id,"exp",expUp);
    }

    bool ret3 =true;
    if(input ->has_bags())
    {
        std::string bagInfoUp;
        GameSpec::BagInfo bagUp =input->bags();
        bagUp.SerializeToString(&bagInfoUp);
        ret3 =UpdateOneCol(id,"bagInfo",bagInfoUp);
    }
    */
    std::string a ="update players set rank =" + std::to_string(rankUp) +",exp =" +std::to_string(expUp) + ",bagInfo = '" + bagInfoUp + "' where id =" + std::to_string(id);
    return updateFun(a);
}


bool MySqlMgr::UpdateRank(int id,int rank) //3. 公有  update 单一字段 ：玩家的排名
{
    return UpdateOneCol(id,"rank",rank);
}

bool MySqlMgr::UpdateExp(int id,int exp) //4. 公有  update  单一字段 ：玩家的经验
{
    return UpdateOneCol(id,"exp",exp);
}

bool MySqlMgr::UpdateBagInfo(int id,const std::string& info) //5.公有  update  单一字段 ：玩家的背包信息 背包信息以字符串形式传入
{
    return UpdateOneCol(id,"bagInfo",info);
}

bool MySqlMgr::CreateNewPlayer(GameSpec::Players* input) //6. 公有  创建新的玩家信息 参数为PB的Players类型指针
{
    if(input == nullptr)
        return false;
    int idIst =input->id();
    std::string name =input->name();
    int rankIst =input->rank();
    int expIst =input ->exp();
    GameSpec::BagInfo bagInfoIst =input->bags();
    std::string bagInfoBlob;
    bagInfoIst.SerializeToString(&bagInfoBlob);

    string a ="insert into players (id,playerName,rank,exp,bagInfo) values (" +std::to_string(idIst) + ", '" +name +"'," + std::to_string(rankIst) +"," + std::to_string(expIst) +", '"+ bagInfoBlob +"')";

    return insertFun(a);

}

bool MySqlMgr::UpdateName(int id,const std::string& newName) //7.公有  根据玩家的ID   修改用户姓名 PS：姓名不允许重复
{
    bool bo = false;
    std::string bosql ="select playerName from players where playerName ='" + newName +"'";
    existFun(bosql,bo);
    if(!bo)
    {
        string a ="update players set playerName = '" + newName + "' where id =" + to_string(id);
        return updateFun(a);
    }
    return false;

}

bool MySqlMgr::GetAll(std::map<int,std::pair<int,std::string>>& res)  //8.公有  得到所有玩家的ID
{
    mysqlpp::StoreQueryResult ares;
    string a ="select id,playerName,exp from players";
    int ret=searchFun(a,ares);
    for(int i=0;i< ares.num_rows();++i)
    {
        std::pair<int,std::string> temp = std::make_pair((int) ares[i]["exp"],(std::string)ares[i]["playerName"]);
        res.insert(std::make_pair(ares[i]["id"],temp));
    }
    return ret;
}


int MySqlMgr::GetPlayerId(const string& input) //9.公有  根据玩家的姓名  查询玩家的ID
{
    string a="select id from players where playerName= '" + input +"'";
    mysqlpp::StoreQueryResult res;
    bool ret =searchFun(a,res);
    if(ret && res.num_rows() > 0)
    {
        return res[0]["id"];
    }
    return -1;
}



/*
bool MySqlMgr::UpdateSet(QuerySet& qs)
{
	if(qs.m_id == -1)
	return false;

	string a ="update players ";

	if(qs.m_name)
	a+="set playerName =' " +qs.m_name +"'";

	if(qs.m_rank != -1)
	a+="set rank = " +std::to_string(qs.m_rank);

	if(qs.m_exp != -1)
	a+="set exp = " +std::to_string(qs.m_exp);

	if(qs.m_extendInfo1)
	a+="set extendInfo1 =' " +qs.m_extendInfo1 + "'";

	if(qs.m_extendInfo2)
	a+="set extendInfo2 =' " +qs.m_extendInfo2 + "'";

	if(qs.m_extendInfo3)
	a+="set extendInfo3 =' " +qs.m_extendInfo3 + "'";

	a+=" where id=" + std::to_string(qs.m_id);

}


bool MySqlMgr::CreatePlayer(QuerySet& qs)
{

	if(qs.m_id == -1)
	return false;

	string a ="insert into players (";
	std::vector<string> cols;
	std::vector<string> vals;

	if(qs.m_name)
	{
		cols.push_back("playerName");
		vals.push_back("'qs.m_name'");
	}

	if(qs.m_rank != -1)
	{
		cols.push_back("rank");
		std::string a =std::to_string(qs.m_rank);
		vals.push_back(a);
	}
	if(qs.m_exp != -1)
	{
		cols.push_back("exp");
		std::string a=std::to_string(qs.m_exp);
		vals.push_back("a");
	}

	if(qs.m_extendInfo1)
	{
		cols.push_back("extendInfo1");
		vals.push_back("'qs.m_extendInfo1'");
	}

	if(qs.m_extendInfo2)
	{
		cols.push_back("extendInfo2");
		vals.push_back("'qs.m_extendInfo2'");
	}

	if(qs.m_extendInfo3)
	{
		cols.push_back("extendInfo3");
		vals.push_back("'qs.m_extendInfo3'");
	}

	a+=" where id=" + std::to_string(qs.m_id);

}


bool MySqlMgr::UpdateName(int id,const string& newName)
    string a ="update players set playerName = '" + newName + "' where id =" + to_string(id);
    return updateFun(a);

}

bool MySqlMgr::AddPlayer(int id,const string& newName)
{
    string a ="insert into players(id,playerName) values(" + to_string(id) +",'" + newName + "')";
    return insertFun(a);
}
*/



/*
bool MySqlMgr::UpdatePlayerBagInfo(int id,const std::)
{
    string blob;
    blob.assign(info,len);
    //string a ="update players set playerInfo =(\"" +to_string(mysqlpp::escape)  +blob +"\") where id =" + to_string(id);
    //return updateFun(a);

}

/*
bool MySqlMgr::GetPlayerInfo(int id,string& info)
{
	string a ="select playerInfo from players where id =" + to_string(id);
    	mysqlpp::String resInfo_pp;
    	bool ret =searchFun2(a,resInfo_pp);
	cout << ret <<endl;
    	info =std::string(resInfo_pp.data(),resInfo_pp.length());
    	return ret;

}
*/
void MySqlMgr::Uinit()
{
    return;
}

// add by lay
// 在login server 开机时候启动用于读入Redis缓存
// TODO：明文密码改为存储加盐密码
//bool MySqlMgr::GetAllUserNameAndPwd(std::map<int,std::pair<int,std::string>>& res)
//{
//    mysqlpp::StoreQueryResult ares;
//    string a ="select id,playerName,exp from players";
//    int ret=searchFun(a,ares);
//    for(int i=0;i< ares.num_rows();++i)
//    {
//        std::pair<int,std::string> temp = std::make_pair((int) ares[i]["exp"],(std::string)ares[i]["playerName"]);
//        res.insert(std::make_pair(ares[i]["id"],temp));
//    }
//    return ret;
//}
