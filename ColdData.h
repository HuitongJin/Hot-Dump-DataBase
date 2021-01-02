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
	const int _block_size = 1024;  // 规定每一个block的大小
	
	BSTree* _AVLTree;  // 使用平衡二叉树存放主键
	
	int _num_of_blocks;  // 一共有几个block
	
	int _size_of_line;  // 每一行的列数

	string _db_name;  // 对应的库名

	string _table_name; // 对应的表名

	DBHandler db;

	TableRow* _table_data;  // 对应的表工具类

	void init_AVLTree();  //初始化二叉树

public:
	ColdData(int size, const string& dbName, const string& tableName);

	~ColdData();

	json* query_by_key(KeyType key);  // 查询接口, 考虑多个匹配结果的情况

};
#endif // !_HDSDATABASE_COLDDATA_H

