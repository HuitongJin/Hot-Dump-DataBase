#pragma once

// auther: Jackson Kim

/********************************************************************************
* ����һ�����ݿ��Ԫ�����࣬ʹ��json��ʽ�洢�����Լ���Ӧ��·����                *
* �������������Խ������²�����                                                *
* 1. error_code save(const vector<string>&, const string&) ���²�����Ԫ����     *                                                         *
* 2. error_code open(const string&) �����Ԫ���ݶ�Ӧ���ļ���                  *
* 3. json load() ����Ԫ���ݣ�����һ��json����                                 *
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

// ������궨������룺
#define _TRUE                        0    // ��ȷ����
#define _CREATE_DATABASEMETA_FAILED  13   // �������ݿ�Ԫ����ʧ��
#define _NOT_EXIST_DATABASE          14   // ���������ݿ�

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
		string         _DBMetaFilePath;   // Ԫ���ݵ��ļ�·��

		json           _metaJson;         // ���ڼ���Ԫ���ݵ�json����
}; 
#endif // !_HDSDATABASE_DBMETA_H

