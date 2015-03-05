#pragma once
#include <list>
#include <boost/thread.hpp>

class MysqlConnector;
class SQLConnectorPool
{
public:
	static 	SQLConnectorPool& Instance();
	MysqlConnector* Get();						// 获取连接		
	void Release(MysqlConnector* connector);	// 释放连接
private:
	SQLConnectorPool();
private:
	std::list<MysqlConnector*> _connectors;
	boost::mutex _mutex;
	const int _max;			// 连接池上限
	volatile int _count;    // 已经生成的connector
	enum { MAX_CONNECTOR = 256, };
}