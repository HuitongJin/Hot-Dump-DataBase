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
	BSTree *_AVLTree; // ����ƽ���������ڴ�źͲ�ѯ����
	
	fstream _db_log;  // ���ڴ��Log�ļ�

	DBHandler db; // ��Ӧ�����ݿ�

	TableRow* _hot_data_file = NULL;  // ���ڴ��������

	TableRow* _new_cold_data_file = NULL;  // ���ڴ����ȴ���������
	
	std::vector<string> _table_meta; // ���Ӧ��Ԫ����
	
	int _rowNum = 0;  // ��¼���������м�������
	
	bool is_dumping = false; // ��¼�Ƿ����ڽ���dump����

	string _table_name;  // ��Ӧ�ı���

	string _db_name;  // ��Ӧ�����ݿ���
	
	const int _block_size = 1000; // ÿ��block��Ӧ������

	// ��������������������������޸��߼�
	void InorderTraversal(BSTree* rt);

public:
	HotData(const string& table_name, const std::vector<string> &table_meta, const string& dbName);
	
	~HotData();

	int add_rows(const nlohmann::json& rows_info);  // ����ӿ�

	std::list<int>* query(KeyType key);  // ��ѯ�ӿ�
	
	HotData* dump_to_cold();

	bool is_switch_mode();

	void data_merge();  // ���ݹ鲢

}; 
#endif // !_HDSDATABASE_HOTDATA_H

