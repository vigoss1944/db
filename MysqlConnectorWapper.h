#pragma once

class MysqlConnector;
class MysqlConnectorWapper
{
public:
	MysqlConnectorWapper();
	virtual ~MysqlConnectorWapper();
	MysqlConnector* Get();
private:
	MysqlConnectorWapper(const MysqlConnectorWapper&);
	MysqlConnectorWapper& operator=(const MysqlConnectorWapper&);

	MysqlConnector* _connector;
	bool _isConnected;		
};