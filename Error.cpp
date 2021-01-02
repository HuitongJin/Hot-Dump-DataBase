#include "Error.h"

Error::Error()
{
	initError();
}

Error::Error(int value, const string& message)
{
	initError();
	_error_value   = value;
	_error_message = message;

#ifdef _DEBUG
	ErrorMap::iterator found = _errMap.find(value);
	if (found != _errMap.end())
	{
		assert(found->second == _error_message);
	}
#endif // _DEBUG
	_errMap[_error_value] = _error_message;
}

Error::operator int()
{
	return _error_value;
}

string Error::GetErrorString(int value)
{
	ErrorMap::iterator found = _errMap.find(value);
	if (found == _errMap.end())
	{
		assert(false);
		return "";
	}
	else
	{
		return found->second;
	}
}

void Error::initError()
{
	_errMap.insert(std::pair<int, string>(1, "参数错误"));
	_errMap.insert(std::pair<int, string>(2, "从某行开始读取时，传入参数超限"));
	_errMap.insert(std::pair<int, string>(3, "文件写入出错"));
	_errMap.insert(std::pair<int, string>(4, "文件打开失败"));
	_errMap.insert(std::pair<int, string>(5, "未打开文件"));
	_errMap.insert(std::pair<int, string>(6, "文件读取失败"));
	_errMap.insert(std::pair<int, string>(7, "文件关闭失败"));
	_errMap.insert(std::pair<int, string>(8, "数据库已存在"));
	_errMap.insert(std::pair<int, string>(9, "表已存在"));
	_errMap.insert(std::pair<int, string>(10, "数据库打开失败"));
	_errMap.insert(std::pair<int, string>(11, "创建元数据失败"));
	_errMap.insert(std::pair<int, string>(12, "删除表失败"));
	_errMap.insert(std::pair<int, string>(13, "创建数据库元数据失败"));
	_errMap.insert(std::pair<int, string>(14, "不存在数据库"));
}