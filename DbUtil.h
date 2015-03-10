#pragma once
#include <string>
#include <mysql.h>
#include "CodeConverter.h"
#include <stdio.h>

namespace DbUtil
{

//输入绑定
inline void BindInInt(MYSQL_BIND& bind, int* data, unsigned long* len, my_bool* is_null)
{
	bind.buffer_type= MYSQL_TYPE_LONG;
	bind.buffer= (char *)data;
	bind.is_null = is_null;
	bind.length = len;
}	

inline void BindInShort(MYSQL_BIND& bind, short* data, unsigned long* len, my_bool* is_null)
{
	bind.buffer_type = MYSQL_TYPE_FLOAT;
	bind.buffer = (char *)data;
	bind.is_null = is_null;
	bind.length = len; 
}

inline void BindInFloat(MYSQL_BIND& bind, float* data, unsigned long* len, my_bool* is_null)
{
	bind.buffer_type= MYSQL_TYPE_FLOAT;
	bind.buffer= (char *)data;
	bind.is_null = is_null;
	bind.length = len;
}

inline void BindInDouble(MYSQL_BIND& bind, double* data, unsigned long* len, my_bool* is_null)
{
	bind.buffer_type= MYSQL_TYPE_DOUBLE;
	bind.buffer= (char *)data;
	bind.is_null = is_null;
	bind.length = len;
}

inline void BindInString(MYSQL_BIND& bind, char* data, int bufLen, unsigned long* len, my_bool* is_null)
{
	bind.buffer_type = MYSQL_TYPE_STRING;
	bind.buffer = (char *)data;
	bind.buffer_length = bufLen;
	bind.is_null = is_null;
	bind.length = len;
}

inline void BindInString(MYSQL_BIND& bind, std::string* data, unsigned long* len, my_bool* is_null)
{
    *len = data->size();
	bind.buffer_type = MYSQL_TYPE_STRING;
	bind.buffer = (char *)data->c_str();
	bind.buffer_length = data->size();
	bind.is_null = is_null;
	bind.length = len;
}

// 输出绑定
inline void BindOutInt(MYSQL_BIND& bind, int* data, unsigned long* len, my_bool* is_null)
{
    bind.buffer_type = MYSQL_TYPE_LONG;
    bind.buffer = (char *)data;
    bind.is_null = is_null;
    bind.length = len;
}

inline void BindOutShort(MYSQL_BIND& bind, short* data, unsigned long* len, my_bool* is_null)
{
    bind.buffer_type = MYSQL_TYPE_SHORT;
    bind.buffer = (char *)data;
    bind.is_null = is_null;
    bind.length = len;
}

inline void BindOutString(MYSQL_BIND& bind, std::string* data, unsigned long* len, my_bool* is_null)
{
    bind.buffer_type = MYSQL_TYPE_STRING;
    bind.buffer = (char *)data->c_str();
    bind.buffer_length = data->capacity();
    bind.is_null = is_null;
    bind.length = len;
}

inline void BindOutString(MYSQL_BIND& bind, char* data, int bufLen, unsigned long* len, my_bool* is_null)
{
    bind.buffer_type = MYSQL_TYPE_STRING;
    bind.buffer = (char *)data;
    bind.buffer_length = bufLen;
    bind.is_null = is_null;
    bind.length = len;
}

inline void BindOutFloat(MYSQL_BIND& bind, float* data, unsigned long* len, my_bool* is_null)
{
    bind.buffer_type = MYSQL_TYPE_FLOAT;
    bind.buffer = (char *)data;
    bind.is_null = is_null;
    bind.length = len;
}

inline void BindOutDouble(MYSQL_BIND& bind, double* data, unsigned long* len, my_bool* is_null)
{
    bind.buffer_type = MYSQL_TYPE_DOUBLE;
    bind.buffer = (char *)data;
    bind.is_null = is_null;
    bind.length = len;
}

inline std::string Get(char* value, int lenght, my_bool is_null, std::string strDefault="")
{
    if (is_null) return strDefault;
    return std::string(value, lenght);
}

}    // namespace DbUtil