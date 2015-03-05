#pragma once
#include <mysql.h>
#include <string>

class MysqlConnector
{
public:
	MysqlConnector(const std::string& host, const std::string& username, const std::string& password, const std::string& dbname, int port=3306);
	virtual ~MysqlConnector;
	bool Connect();
	bool Execute(const std::string& sql);
	MYSQL* Get();
private:
	MYSQL _conn;
	std::string _host;
	std::string _username;
	std::string _password;
	std::string _dbname;
	int port;	
};