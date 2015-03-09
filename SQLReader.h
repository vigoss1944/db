#pragma once
#include <mysql.h>
#include <string>

class MysqlConnector;
class SQLReader
{
public:
	SQLReader(MysqlConnector* connector);
	virtual ~SQLReader() = 0;

	virtual bool Execute();

protected:
	virtual std::string GetSQL() = 0;
	virtual void BindInParam(MYSQL_BIND* bind, unsigned long* lenght, my_bool* is_null) = 0;
	virtual void BindOutParam(MYSQL_BIND* bind, unsigned long* lenght, my_bool* is_null) = 0;
	virtual bool GetRecord(unsigned long* lenght, my_bool* is_null) = 0;
	virtual void DoFinish(int count){}

private:
	bool StmtInit();
	bool StmtPrepare();
	bool StmtBindInParam();
	bool StmtExecute();
	bool StmtBindOutParam();
	bool StmtGetResult();
	void InitBindParam(int type, int size);
protected:
	enum { BIND_IN_PARAM = 1, BIND_OUT_PARAM = 2,};
	MysqlConnector* _connector;
	MYSQL_STMT* _stmt;
	MYSQL_BIND* _inBind;
	MYSQL_BIND* _outBind;
	MYSQL_RES* _prepare_meta_result;
	int _param_count, _column_count;
	unsigned long* _inLength;
	my_bool* _inIsNull;
	unsigned long* _outLength;
	my_bool* _outIsNull;
};