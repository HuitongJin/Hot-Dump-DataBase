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

}