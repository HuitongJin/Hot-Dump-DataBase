#pragma once

// auther: Jackson Kim

/********************************************************************************
* 这是一个数据库的元数据类，使用json格式存储表名以及对应的路径。                *
* 在这个类中你可以进行以下操作：                                                *
* 1. error_code save(const vector<string>&, const string&) 更新并保存元数据     *                                                         *
* 2. error_code open(const string&) 打开这个元数据对应的文件。                  *
* 3. json load() 加载元数据，返回一个json对象。                                 *
*********************************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>

#ifndef _HDSDATABASE_DBMETA_H
#define _HDSDATABASE_DBMETA_H

using std::string;
using std::vector;
using std::ios_base;
using json = nlohmann::json;
typedef int error_code;

// 在这里宏定义错误码：
#define _TRUE                        0    // 正确操作
#define _CREATE_DATABASEMETA_FAILED  13   // 创建数据库元数据失败
#define _NOT_EXIST_DATABASE          14   // 不存在数据库

class DBMeta
{
	public:
		DBMeta();

		DBMeta(const string& DBMetaFilePath_);

		~DBMeta();

		error_code save(const vector<string>& tableName_, const string& FilePath_);

		error_code open(const string& DBMetaFilePath_);

		json load();

	private:
		string         _DBMetaFilePath;   // 元数据的文件路径

		json           _metaJson;         // 用于加载元数据的json对象
}; 
#endif // !_HDSDATABASE_DBMETA_H

