#include "HotData.h"
HotData::HotData( const string& table_name, const std::vector<string>& table_meta, const string& dbName)
{
	_table_name = table_name;
	_db_name = dbName;
	_table_meta = table_meta;

	string hot_data_table_name = _table_name + "HotData";
	string new_cold_data_name = _table_name + "NewCold";

	// �������ݿ�
	db.create(_db_name);
	// ���������ݱ�
	db.createTable(_table_name, _table_meta);
	// ���������ݱ�,�µ������ݱ�
	db.createTable(hot_data_table_name, _table_meta);
	db.createTable(new_cold_data_name, _table_meta);
	_hot_data_file = new TableRow(_db_name, hot_data_table_name);
	_new_cold_data_file = new TableRow(_db_name, new_cold_data_name);
	// ����Log�ļ�
	//TODO
}

HotData::~HotData()
{
	db.closeTable(_table_name);
	dump_to_cold();  // �˳�ʱ����Ҫ��������ȴ
	if(_AVLTree != NULL)
		delete _AVLTree;
	delete _hot_data_file;
}

// �û���������ݣ�֧��һ����������
int HotData::add_rows(const nlohmann::json& rows_info)
{
	_hot_data_file->write(rows_info);
	// ���¶�����,����Լ��json�ĵ�һ��Ԫ����Ϊ����
	for (int i = 0; i < rows_info.size(); i++)
	{
		_rowNum++;
		InsertBST(_AVLTree, rows_info[i][0], _rowNum);
	}
	// �����������趨��blockʱ������һ����ȴ
	if (_rowNum >= _block_size)
	{
		dump_to_cold();
	}
}

void HotData::InorderTraversal(BSTree* rt) {	//�������ʹ�ö������������
	if (rt == NULL || _rowNum == 0)	
		return;
	InorderTraversal(rt->left);
	json* data = _hot_data_file->read(rt->data, 1); // ��ȡ����
	_new_cold_data_file->write(*data);  // ������д���µ���ȴ����
	_rowNum--;
	//cout << rt->key << " ";
	InorderTraversal(rt->right);
}

// �¾������ļ��鲢
void HotData::data_merge()
{
	string old_cold_path = "D:\\HDSDataBase\\" + _db_name + "\\" + _table_name + "\\data.dat";
	string new_cold_path = "D:\\HDSDataBase\\" + _db_name + "\\" + _table_name + "NewCold\\data.dat";
	string tmp_path = "D:\\HDSDataBase\\" + _db_name + "\\" + _table_name + "\\tmpdata.dat";   // ��Ϊת�Ƶ���ʱ�ļ�

	// ���ļ�
	std::ifstream old_cold(old_cold_path, ios_base::in | ios_base::app);
	std::ifstream new_cold(new_cold_path, ios_base::in | ios_base::app);
	std::ofstream tmp(tmp_path, ios_base::out | ios_base::app);
	
	// ��ʼ���ļ�ָ��ƫ��
	int old_offset = 0;
	int new_offfset = 0;
	int tmp_offset = 0;

	// ��������
	int size = _table_meta.size();
	int* old_array = new int[size];
	int* new_array = new int[size];

	// �ƶ�ָ�룬׼����ȡ���ݣ�
	old_cold.seekg(old_offset++, ios_base::beg);
	new_cold.seekg(new_offfset++, ios_base::beg);

	while (true)
	{
		if (old_cold.eof() || new_cold.eof() )  // �������һ���ļ�������
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
	// ���ˣ������ļ�������������ɹ鲢����������ת������
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

// ��ȴ
HotData* HotData::dump_to_cold()
{
	is_dumping = true;
	// ʹ���������ʹ����������ȴ������
	InorderTraversal(_AVLTree);
	_hot_data_file->clear(); // ��ԭ�ȵ��������
	_rowNum = 0; // ���õ�ǰ����������Ϊ0
	if (_AVLTree)
	{
		delete _AVLTree;  // ��ƽ����������
	}
	is_dumping = false;
	// ���µ������ݺ�ԭ�ȵ������ݽ��й鲢�����������ݷ���һ��
	data_merge();
}

bool HotData::is_switch_mode()
{
	return is_dumping;
}

// ����������ѯ��Ӧ����������
std::list<int>* HotData::query(KeyType key)
{
	std::list<int> map_data;
	BSTree* node = SearchNode(_AVLTree, key);
	while (node != NULL)
	{
		map_data.push_back(node->data);
		// ����Լ������������ֵ���ʱ���������ӵ�λ�ã�
		// ����������������ҵ�����ƥ�������
		node = SearchNode(node->left, key);  
	}
}
