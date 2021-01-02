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
	_errMap.insert(std::pair<int, string>(1, "��������"));
	_errMap.insert(std::pair<int, string>(2, "��ĳ�п�ʼ��ȡʱ�������������"));
	_errMap.insert(std::pair<int, string>(3, "�ļ�д�����"));
	_errMap.insert(std::pair<int, string>(4, "�ļ���ʧ��"));
	_errMap.insert(std::pair<int, string>(5, "δ���ļ�"));
	_errMap.insert(std::pair<int, string>(6, "�ļ���ȡʧ��"));
	_errMap.insert(std::pair<int, string>(7, "�ļ��ر�ʧ��"));
	_errMap.insert(std::pair<int, string>(8, "���ݿ��Ѵ���"));
	_errMap.insert(std::pair<int, string>(9, "���Ѵ���"));
	_errMap.insert(std::pair<int, string>(10, "���ݿ��ʧ��"));
	_errMap.insert(std::pair<int, string>(11, "����Ԫ����ʧ��"));
	_errMap.insert(std::pair<int, string>(12, "ɾ����ʧ��"));
	_errMap.insert(std::pair<int, string>(13, "�������ݿ�Ԫ����ʧ��"));
	_errMap.insert(std::pair<int, string>(14, "���������ݿ�"));
}