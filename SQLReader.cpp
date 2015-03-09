#include "SQLReader.h"
#include "MysqlConnector.h"
#include <string.h>
#include <stdio.h>
#include <glog/logging.h>

SQLReader::SQLReader(MysqlConnector* connector) : _connector(connector), _stmt(0)
	, _inBind(0), _prepare_meta_result(0), _param_count(0), _column_count(0), _inLength(0)
	, _inIsNull(0), _outBind(0), _outLength(0), _outIsNull(0)
{
}

SQLReader::~SQLReader()
{
	if (_stmt)
		mysql_stmt_close(_stmt);
	if (_inBind)
		delete [] _inBind;
	if (_inLength)
		delete [] _inLength;
	if (_inIsNull)
		delete [] _inIsNull;

	if (_outBind)
		delete [] _outBind;
	if (_outLength)
		delete [] _outLength;
	if (_outIsNull)
		delete [] _outIsNull;

	if (_prepare_meta_result)
		mysql_free_result(_prepare_meta_result);
}

bool SQLReader::Execute()
{
	if (StmtInit() && StmtPrepare() && StmtBindInParam() && StmtExecute() && StmtBindOutParam() && StmtGetResult() ) return true;

	return false;
}


bool SQLReader::StmtInit()
{
	_stmt = mysql_stmt_init(_connector->Get() );
	if (_stmt) return true;
	LOG(ERROR) << "mysql_stmt_init(), out of memory";
	return false;
}

bool SQLReader::StmtPrepare()
{
	std::string sql = GetSQL();
	if (!mysql_stmt_prepare(_stmt, sql.c_str(), sql.size() ) ) return true;
	LOG(ERROR) << "mysql_stmt_prepare() failed\n" << mysql_stmt_error(_stmt);
	return false;
}

bool SQLReader::StmtBindInParam()
{
	_param_count = mysql_stmt_param_count(_stmt);
	LOG(INFO) << "total parameters in SELECT:" << _param_count;
	if (_param_count > 0)
	{
		InitBindParam(BIND_IN_PARAM, _param_count);
		BindInParam(_inBind, _inLength, _inIsNull);
		if (mysql_stmt_bind_param(_stmt, _inBind))
		{
			LOG(ERROR) << "mysql_stmt_bind_param failed " << mysql_stmt_error(_stmt);
			return false;
		}
	}
	_prepare_meta_result = mysql_stmt_result_metadata(_stmt);
	if (!_prepare_meta_result)
	{
		LOG(ERROR) << "mysql_stmt_result_metadata(), returned no meta information\n" << mysql_stmt_error(_stmt);
		return false;
	}
	_column_count = mysql_num_fields(_prepare_meta_result);
	LOG(INFO) << "total columns in SELECT statement:" << _column_count;
	return true; 
}

bool SQLReader::StmtExecute()
{
	if (! mysql_stmt_execute(_stmt) ) return true;
	LOG(ERROR) << "mysql_stmt_execute " << mysql_stmt_error(_stmt);
	return false;
}

bool SQLReader::StmtBindOutParam()
{
	InitBindParam(BIND_OUT_PARAM, _column_count);
	BindOutParam(_outBind, _outLength, _outIsNull);
	if (mysql_stmt_bind_result(_stmt, _outBind))
	{
		LOG(ERROR) << "mysql_stmt_bind_result() failed\n"  << mysql_stmt_error(_stmt);
        return false;		
	}
	return true;
}

void SQLReader::InitBindParam(int type, int size)
{
	if (type == BIND_IN_PARAM)
	{
		_inBind = new MYSQL_BIND[size];
		memset(_inBind, 0, sizeof(MYSQL_BIND)*size);
		_inLength = new unsigned long[size];
		memset(_inLength, 0, sizeof(unsigned long)*size);
		_inIsNull = new my_bool[size];
		memset(_inIsNull, 0, sizeof(my_bool)*size);
	}
	else if (type == BIND_OUT_PARAM)
	{
		_outBind = new MYSQL_BIND[size];
		memset(_outBind, 0, sizeof(MYSQL_BIND)*size);
		_outLength = new unsigned long[size];
		memset(_outLength, 0, sizeof(unsigned long)*size);
		_outIsNull = new my_bool[size];
		memset(_outIsNull, 0, sizeof(my_bool)*size);
	}	
}

bool SQLReader::StmtGetResult()
{
	if (mysql_stmt_store_result(_stmt))
	{
		LOG(ERROR) << "mysql_stmt_store_result() failed\n" << mysql_stmt_error(_stmt);
		false;
	}

	int count(0);
	while (!mysql_stmt_fetch(_stmt))
	{
		GetRecord(_outLength, _outIsNull);
		++count;
	}
	DoFinish(count);
	LOG(INFO) << "Fetching results ..." << count;
	return true;
}





