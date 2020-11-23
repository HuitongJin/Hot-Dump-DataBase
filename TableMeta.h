#pragma once

// auther: Jackson Kim

/********************************************************************************
* 这是一个表的元数据类，使用json格式存储列名以及对应的下标，以及主键。          *
* 在这个类中你可以进行以下操作：                                                *
* 1. error_code save(const vector<string>&,const string& , const string& ) 保存 *
*    并更新元数据。                                                             *
* 2. error_code open(const string&) 打开这个元数据对应的文件。                  *
* 3. json load() 加载元数据，返回一个json对象。                                 *
*********************************************************************************/

// 需要特别注意的是：主键会作为json的一个成员放在最后，所以加载元数据的时候，
// 注意最后一个元素是主键。

#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>

#ifndef _HDSDATABASE_TABLEMETA_H
#define _HDSDATABASE_TABLEMETA_H

using std::string;
using std::vector;
using std::ios_base;
using json = nlohmann::json;
typedef int error_code;

// 这里宏定义错误码：
#define _TRUE                     0
#define _CREATE_TABLEMETA_FAILED  11

class TableMeta
{
	public:
		~TableMeta();

		error_code save(const vector<string>& columnName_,const string& primaryKey_, 
			            const string& metaFilePath_);

		error_code open(const string& metaFilePath_);

		json load(const string& filePath_);

	private:		
		string         _primaryKey;    // 主键
		  
		string         _metaFilePath;  // 元数据文件的路径

		json           _metaJson;      // 加载元数据的json对象
}; 
#endif // !_HDSDATABASE_TABLEMETA_H

