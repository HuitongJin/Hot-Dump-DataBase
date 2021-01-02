#include "ColdData.h"

ColdData::ColdData(int size, const string& dbName, const string& tableName)
{
	_db_name = dbName;
	_size_of_line = size;
	_table_name = tableName;

	db.open(_db_name);
	_table_data = new TableRow(_db_name, _table_name);

	init_AVLTree();
}

ColdData::~ColdData()
{
	db.closeTable(_table_name);
	if (_AVLTree!= NULL)
	{
		delete _AVLTree;
	}
	if (_table_data != NULL)
	{
		delete _table_data;
	}
}

void ColdData::init_AVLTree()
{
	int offset = 0;  // 每一个block第一组数据的偏移量
	int* array = new int[_size_of_line];
	string cold_path = "D:\\HDSDataBase\\" + _db_name + "\\" + _table_name + "\\data.dat";
	std::ifstream cold_data(cold_path, ios_base::in | ios_base::app);  // 读取冷数据文件内容
	cold_data.seekg(offset, ios_base::beg);
	// 循环读取每个block，更新二叉树
	while (!cold_data.eof())
	{
		cold_data.read((char*)array, _size_of_line * sizeof(int));
		InsertBST(_AVLTree, array[0], offset);
		offset += _block_size;
		cold_data.seekg(offset, ios_base::beg);
	}
}

json* ColdData::query_by_key(KeyType key)
{
	json* ansJson = new json;
	*ansJson = json::array();
	BSTree* node = SearchNode(_AVLTree, key);
	int* array = new int[_size_of_line];
	int init_offset = node->data;
	string cold_path = "D:\\HDSDataBase\\" + _db_name + "\\" + _table_name + "\\data.dat";
	std::ifstream cold_data(cold_path, ios_base::in | ios_base::app);  // 读取冷数据文件内容
	cold_data.seekg(init_offset++, ios_base::beg);
	while (!cold_data.eof())
	{
		cold_data.read((char*)array, _size_of_line * sizeof(int));
		json* infoJson = new json;
		*infoJson = json::array();
		for (int i = 0; i < _size_of_line; i++)
		{
			infoJson->push_back(array[i]);
		}
		ansJson->push_back(*infoJson);
		delete infoJson;
		cold_data.seekg(init_offset++, ios_base::beg);
	}
	if (cold_data.is_open())
	{
		cold_data.close();
	}
	return ansJson;
}

