#include "MysqlConnecctorWapper.h"
#include "SQLConnectorPool.h"

MysqlConnecctorWapper::MysqlConnecctorWapper() : _connector(0), _isConnected(false)
{
}

MysqlConnecctorWapper::~MysqlConnecctorWapper()
{
	if (_isConnected)
		SQLConnectorPool::Instance().Release(_connector);
}

MysqlConnecctor* MysqlConnecctorWapper::Get()
{
	if (!_isConnected)
	{
		_connector = SQLConnectorPool::Instance().Get();
		_isConnected = true;
	}
	return _connector;
}