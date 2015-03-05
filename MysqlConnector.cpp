#include "MysqlConnector.h"
#include <stdio.h>
#include <glog/logging.h>

MysqlConnector::MysqlConnector():_host(host), _username(username), _password(password), _dbname(dbname), _port(port)
{
	mysql_init(&_conn);
}

MysqlConnector::~MysqlConnector()
{
	mysql_close(&_conn);
}

bool MysqlConnector::Connect()
{
	if (!mysql_real_connect(&_conn, _host.c_str(), _username.c_str(), _password.c_str(), _dbname.c_str(), _port, NULL, 0) )
	{
		LOG(ERROR) << "Connect Error:" << mysql_error(&_conn);
		return false;
	}
	if (!mysql_set_character_set(&_conn, "utf8") )
	{
		LOG(INFO) << "mysql character set:" << mysql_character_set_name(&_conn);
	}
	return ! mysql_select_db(&_conn, _dbname.c_str() );
}

bool MysqlConnector::Execute(const std::string& sql)
{
	return !mysql_real_query( Get(), sql.c_str(), sql.size() );
}

MYSQL* MysqlConnector::Get()
{
	return &_conn;
}