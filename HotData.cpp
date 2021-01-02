#include "HotData.h"
HotData::HotData( const string& table_name, const std::vector<string>& table_meta, const string& dbName)
{
	_table_name = table_name;
	_db_name = dbName;
	_table_meta = table_meta;

	string hot_data_table_name = _table_name + "HotData";
	string new_cold_data_name = _table_name + "NewCold";

	// 创建数据库
	db.create(_db_name);
	// 创建冷数据表
	db.createTable(_table_name, _table_meta);
	// 创建热数据表,新的冷数据表
	db.createTable(hot_data_table_name, _table_meta);
	db.createTable(new_cold_data_name, _table_meta);
	_hot_data_file = new TableRow(_db_name, hot_data_table_name);
	_new_cold_data_file = new TableRow(_db_name, new_cold_data_name);
	// 创建Log文件
	//TODO
}

HotData::~HotData()
{
	db.closeTable(_table_name);
	dump_to_cold();  // 退出时，需要将数据冷却
	if(_AVLTree != NULL)
		delete _AVLTree;
	delete _hot_data_file;
}

// 用户添加热数据，支持一条或多条添加
int HotData::add_rows(const nlohmann::json& rows_info)
{
	_hot_data_file->write(rows_info);
	// 更新二叉树,我们约定json的第一个元素作为主键
	for (int i = 0; i < rows_info.size(); i++)
	{
		_rowNum++;
		InsertBST(_AVLTree, rows_info[i][0], _rowNum);
	}
	// 当行数超过设定的block时，就做一次冷却
	if (_rowNum >= _block_size)
	{
		dump_to_cold();
	}
}

void HotData::InorderTraversal(BSTree* rt) {	//中序遍历使得二叉树有序输出
	if (rt == NULL || _rowNum == 0)	
		return;
	InorderTraversal(rt->left);
	json* data = _hot_data_file->read(rt->data, 1); // 读取数据
	_new_cold_data_file->write(*data);  // 将数据写入新的冷却表中
	_rowNum--;
	//cout << rt->key << " ";
	InorderTraversal(rt->right);
}

// 新旧数据文件归并
void HotData::data_merge()
{
	string old_cold_path = "D:\\HDSDataBase\\" + _db_name + "\\" + _table_name + "\\data.dat";
	string new_cold_path = "D:\\HDSDataBase\\" + _db_name + "\\" + _table_name + "NewCold\\data.dat";
	string tmp_path = "D:\\HDSDataBase\\" + _db_name + "\\" + _table_name + "\\tmpdata.dat";   // 作为转移的临时文件

	// 打开文件
	std::ifstream old_cold(old_cold_path, ios_base::in | ios_base::app);
	std::ifstream new_cold(new_cold_path, ios_base::in | ios_base::app);
	std::ofstream tmp(tmp_path, ios_base::out | ios_base::app);
	
	// 初始化文件指针偏移
	int old_offset = 0;
	int new_offfset = 0;
	int tmp_offset = 0;

	// 设置数组
	int size = _table_meta.size();
	int* old_array = new int[size];
	int* new_array = new int[size];

	// 移动指针，准备读取数据；
	old_cold.seekg(old_offset++, ios_base::beg);
	new_cold.seekg(new_offfset++, ios_base::beg);

	while (true)
	{
		if (old_cold.eof() || new_cold.eof() )  // 如果其中一个文件结束了
		{
			break;
		}
		old_cold.read((char*)old_array, size * sizeof(int));
		new_cold.read((char*)new_array, size * sizeof(int));
		if (old_array[0] <= new_array[0])
		{
			tmp.seekp(tmp_offset, ios_base::end);
			tmp.write((char*)old_array, size * sizeof(int));
			old_cold.seekg(old_offset++, ios_base::beg);
		}
		else
		{
			tmp.seekp(tmp_offset, ios_base::end);
			tmp.write((char*)new_array, size * sizeof(int));
			new_cold.seekg(new_offfset++, ios_base::beg);
		}
	}

	while (!new_cold.eof())
	{
		new_cold.read((char*)new_array, size * sizeof(int));
		tmp.seekp(tmp_offset, ios_base::end);
		tmp.write((char*)new_array, size * sizeof(int));
		new_cold.seekg(new_offfset++, ios_base::beg);
	}

	while (!new_cold.eof())
	{
		old_cold.read((char*)old_array, size * sizeof(int));
		tmp.seekp(tmp_offset, ios_base::end);
		tmp.write((char*)old_array, size * sizeof(int));
		old_cold.seekg(old_offset++, ios_base::beg);
	}
	if (new_cold.is_open())
	{
		new_cold.clear();
		new_cold.close();
	}
	if (old_cold.is_open())
	{
		old_cold.close();
	}
	if (tmp.is_open())
	{
		tmp.close();
	}

	//===========================================================
	// 至此，两个文件的冷数据已完成归并，接下来是转移数据
	std::ofstream target_cold(old_cold_path, ios_base::out | ios_base::app);
	std::ifstream tmp_cold(tmp_path, ios_base::in | ios_base::app);

	old_offset = 0;
	tmp_offset = 0;
	tmp_cold.seekg(tmp_offset++, size * sizeof(int));
	while (!tmp_cold.eof())
	{
		tmp_cold.read((char*)new_array, size * sizeof(int));
		target_cold.seekp(old_offset, ios_base::end);
		target_cold.write((char*)new_array, size * sizeof(int));
		tmp_cold.seekg(tmp_offset++, size * sizeof(int));
	}
	if (tmp_cold.is_open())
	{
		tmp_cold.clear();
		tmp_cold.close();
	}
	if (target_cold.is_open())
	{
		target_cold.close();
	}
}

// 冷却
HotData* HotData::dump_to_cold()
{
	is_dumping = true;
	// 使用中序遍历使得热数据冷却后有序
	InorderTraversal(_AVLTree);
	_hot_data_file->clear(); // 将原先的数据清空
	_rowNum = 0; // 设置当前热数据行数为0
	if (_AVLTree)
	{
		delete _AVLTree;  // 将平衡二叉树清空
	}
	is_dumping = false;
	// 将新的冷数据和原先的冷数据进行归并排序后跟冷数据放在一起。
	data_merge();
}

bool HotData::is_switch_mode()
{
	return is_dumping;
}

// 根据主键查询对应的所有数据
std::list<int>* HotData::query(KeyType key)
{
	std::list<int> map_data;
	BSTree* node = SearchNode(_AVLTree, key);
	while (node != NULL)
	{
		map_data.push_back(node->data);
		// 我们约定当两个主键值相等时，放在左孩子的位置，
		// 因此沿左子树可以找到所有匹配的主键
		node = SearchNode(node->left, key);  
	}
}
