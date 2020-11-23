#pragma once

// auther: Jackson Kim

/********************************************************************************
* ����һ�����Ԫ�����࣬ʹ��json��ʽ�洢�����Լ���Ӧ���±꣬�Լ�������          *
* �������������Խ������²�����                                                *
* 1. error_code save(const vector<string>&,const string& , const string& ) ���� *
*    ������Ԫ���ݡ�                                                             *
* 2. error_code open(const string&) �����Ԫ���ݶ�Ӧ���ļ���                  *
* 3. json load() ����Ԫ���ݣ�����һ��json����                                 *
*********************************************************************************/

// ��Ҫ�ر�ע����ǣ���������Ϊjson��һ����Ա����������Լ���Ԫ���ݵ�ʱ��
// ע�����һ��Ԫ����������

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

// ����궨������룺
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
		string         _primaryKey;    // ����
		  
		string         _metaFilePath;  // Ԫ�����ļ���·��

		json           _metaJson;      // ����Ԫ���ݵ�json����
}; 
#endif // !_HDSDATABASE_TABLEMETA_H

