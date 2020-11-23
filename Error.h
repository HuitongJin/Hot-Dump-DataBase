#pragma once
#include <string>
#include <cassert>
#include <map>

#ifndef _HDSDATABASE_ERROR_H
#define _HDSDATABASE_ERROR_H

using std::string;
using std::map;

class Error
{
	public:
		Error();

		Error(int value, const string& message);
		
		operator int();

		string GetErrorString(int value);

		void initError();

	private:
		typedef map<int, string> ErrorMap;
 		
		int	    _error_value;

		string  _error_message;

		ErrorMap _errMap;
};
#endif // !_HDSDATABASE_ERROR_H

