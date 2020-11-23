#pragma once

// auther: Jackson Kim

/********************************************************************************
* ����һ�����ݿ�����࣬���ڲ���ĳ�����ݿ⣬������ɾ��ġ�                      *
* �������������Խ������²�����                                                *
* 1. error_code create(const string& DBName_) ����һ�����ݿ⡣                  *
* 2. error_code createTable(const string& ,const vector<string>&,const string&) *
*    ����һ��������Ҫ����������������Լ�������                               *
* 3. error_code createTable(const string&, const vector<string>&��ֻʹ�ñ�����  *
*    ������һ����                                                             *
* 4. TableHandler openTable(const string&) ���ݱ�����һ����                 *
* 5. error_code open(const string& DBName_) �������ݿ�����һ���Ѿ����ڵ����ݿ�*
* 6. error_code closeTable(const string&) ���ݱ����ر�һ���Ѿ��򿪵ı�        *
* 7. error_code drop()ɾ��������ݿ⡣                                          *
* 8. error_code dropTable(const string&) ���ݱ���ɾ��������ݿ��һ����       *
*********************************************************************************/

// ��Ҫ�ر�ע����ǣ����㴴���ı�û�ж�������ʱ����Ĭ�Ͻ���һ��������Ϊ������

#include <direct.h>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>
#include "TableHandler.h"
#include "DBMeta.h"

#ifndef _HDSDATABASE_DBHANDLER_H
#define _HDSDATABASE_DBHANDLER_H

using std::string;
using std::vector;
using json = nlohmann::json;
typedef int error_code;

// ����궨������룺
#define _TRUE                  0
#define _DATABASE_EXIST        8
#define _TABLE_EXIST           9
#define _OPEN_DATABASE_FAILED  10 
#define _NOT_EXIST_DATABASE    14   // ���������ݿ�

class DBHandler
{
	public:
		DBHandler();
		
		DBHandler(const string& DBName_);
		
		~DBHandler();
		
		error_code create(const string& DBName_);
		
		error_code createTable(const string& tableName_, const vector<string>& columnName_, 
			                   const string& primaryKey_);

		error_code createTable(const string& tableName_, const vector<string>& columnName_);
		
		TableHandler openTable(const string& tableName_);

		error_code open(const string& DBName_);

		error_code closeTable(const string& tableName_);
		
		error_code drop();
		
		error_code dropTable(const string& tableName_);

	private:
		string         _DBName;         // ���ݿ���
		 
		string         _DBPath;         // ���ݿ��ļ����·��
		 
		DBMeta         _dbMeta;         // ���ݿ�Ԫ���������

		TableHandler   _tableHandler;   // ���ݿ��Ӧ�ı���������
		 
		vector<string> _tableName;      // ���������ݿ����б�����vector�����ڸ���Ԫ����
};
#endif // !_HDSDATABASE_DBHANDLER_H