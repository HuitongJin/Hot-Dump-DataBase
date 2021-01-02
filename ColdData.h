#pragma once

// auther: Jackson Kim

#ifndef _HDSDATABASE_COLDDATA_H
#define _HDSDATABASE_COLDDATA_H

#include "TableRow.h"
#include "Error.h"
#include "AVLTree.h"
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ColdData
{
private:
	const int _block_size = 1024;  // �涨ÿһ��block�Ĵ�С
	
	BSTree* _AVLTree;  // ʹ��ƽ��������������
	
	int _num_of_blocks;  // һ���м���block
	
	int _size_of_line;  // ÿһ�е�����

	string _db_name;  // ��Ӧ�Ŀ���

	string _table_name; // ��Ӧ�ı���

	DBHandler db;

	TableRow* _table_data;  // ��Ӧ�ı�����

	void init_AVLTree();  //��ʼ��������

public:
	ColdData(int size, const string& dbName, const string& tableName);

	~ColdData();

	json* query_by_key(KeyType key);  // ��ѯ�ӿ�, ���Ƕ��ƥ���������

};
#endif // !_HDSDATABASE_COLDDATA_H

