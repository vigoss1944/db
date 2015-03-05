#include "SQLConnectorPool.h"
#include "MysqlConnector.h"
#include "GConfig.h"
#include <iostream>
#include <glog/logging.h>

SQLConnectorPool& SQLConnectorPool::Instance()
{
	static SQLConnectorPool _instance;
	return _instance;
}

SQLConnectorPool::SQLConnectorPool() : _max(MAX_CONNECTOR), _count(0)
{
}

MysqlConnector* SQLConnectorPool::Get()
{
	boost::lock_guard<boost::mutex> lock(_mutex);
	if (_connectors.empty())
	{
		// TODO 通过配置文件配置这些信息,GetLocal返回的是引用，外面也需要用引用来接
		STServerConfig& config = GConfig::Instance().GetLocal();
		LOG(INFO) << config.dbConfig.host << ", " << config.dbConfig.user << ", " << config.dbConfig.password  << ", " << config.dbConfig.dbname;
		MysqlConnector* connector(new MysqlConnector(config.dbConfig.host, config.dbConfig.user,
												config.dbConfig.password, config.dbConfig.dbname,
												config.dbConfig.port) );
		if (!connector->Connect())
		{
			LOG(INFO) << "connector->Connect() fail";
			return NULL;
		}
		++_count;
		return connector;
	}
	MysqlConnector* connector = _connectors.front();
	_connectors.pop_front();
	return connector;
	//第1点：本指针指向的内容不会自动释放，但是本指针出了局域作用域就会自动释放
	//第2点：所以在外面可以直接用本指针来操作函数，也可以安全的赋值给本类型指针
}

void SQLConnectorPool::Release(MysqlConnector* connector)
{
	boost::lock_guard<boost::mutex> lock(_mutex);
	if(_count > _max)
	{
		delete connector;
		--_count;
		return;
	}
	_connectors.push_back(connector);
}