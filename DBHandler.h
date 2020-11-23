#pragma once

// auther: Jackson Kim

/********************************************************************************
* 这是一个数据库管理类，用于操作某个数据库，包括增删查改。                      *
* 在这个类中你可以进行以下操作：                                                *
* 1. error_code create(const string& DBName_) 创建一个数据库。                  *
* 2. error_code createTable(const string& ,const vector<string>&,const string&) *
*    创建一个表，你需要传入表名，列名，以及主键。                               *
* 3. error_code createTable(const string&, const vector<string>&）只使用表名列  *
*    名创建一个表。                                                             *
* 4. TableHandler openTable(const string&) 根据表名打开一个表。                 *
* 5. error_code open(const string& DBName_) 根据数据库名打开一个已经存在的数据库*
* 6. error_code closeTable(const string&) 根据表名关闭一个已经打开的表。        *
* 7. error_code drop()删除这个数据库。                                          *
* 8. error_code dropTable(const string&) 根据表名删除这个数据库的一个表。       *
*********************************************************************************/

// 需要特别注意的是：当你创建的表没有定义主键时，将默认将第一个列名作为主键。

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

// 这里宏定义错误码：
#define _TRUE                  0
#define _DATABASE_EXIST        8
#define _TABLE_EXIST           9
#define _OPEN_DATABASE_FAILED  10 
#define _NOT_EXIST_DATABASE    14   // 不存在数据库

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
		string         _DBName;         // 数据库名
		 
		string         _DBPath;         // 数据库文件存放路径
		 
		DBMeta         _dbMeta;         // 数据库元数据类对象

		TableHandler   _tableHandler;   // 数据库对应的表管理类对象
		 
		vector<string> _tableName;      // 存放这个数据库所有表名的vector，用于更新元数据
};
#endif // !_HDSDATABASE_DBHANDLER_H