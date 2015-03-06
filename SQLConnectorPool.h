#pragma once
#include <list>
#include <boost/thread.hpp>
/*外面只有一份静态实例，但是会用许多Get()方法，所以将Get()里面设置为线程安全的*/
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
};
