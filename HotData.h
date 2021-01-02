#pragma once

// auther: Jackson Kim

#ifndef _HDSDATABASE_HOTDATA_H
#define _HDSDATABASE_HOTDATA_H

#include "TableRow.h"
#include "Error.h"
#include "AVLTree.h"
#include <vector>

class HotData
{
private:
	BSTree *_AVLTree; // 二叉平衡树，用于存放和查询主键
	
	fstream _db_log;  // 用于存放Log文件

	DBHandler db; // 对应的数据库

	TableRow* _hot_data_file = NULL;  // 用于存放热数据

	TableRow* _new_cold_data_file = NULL;  // 用于存放冷却后的热数据
	
	std::vector<string> _table_meta; // 表对应的元数据
	
	int _rowNum = 0;  // 记录热数据中有几行数据
	
	bool is_dumping = false; // 记录是否正在进行dump操作

	string _table_name;  // 对应的表名

	string _db_name;  // 对应的数据库名
	
	const int _block_size = 1000; // 每个block对应的数量

	// 中序遍历二叉树，方便在这里修改逻辑
	void InorderTraversal(BSTree* rt);

public:
	HotData(const string& table_name, const std::vector<string> &table_meta, const string& dbName);
	
	~HotData();

	int add_rows(const nlohmann::json& rows_info);  // 插入接口

	std::list<int>* query(KeyType key);  // 查询接口
	
	HotData* dump_to_cold();

	bool is_switch_mode();

	void data_merge();  // 数据归并

}; 
#endif // !_HDSDATABASE_HOTDATA_H

