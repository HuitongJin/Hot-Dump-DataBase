#include "DBHandler.h"

DBHandler::DBHandler(){}

DBHandler::DBHandler(const string& DBName_)
{
	_DBName = DBName_;
	_DBPath = "D:\\HDSDataBase\\" + _DBName;
	_mkdir(_DBPath.c_str());
	_dbMeta = DBMeta(_DBPath + "\\meta.json");
}

DBHandler::~DBHandler()
{
	_DBName = "";
	_DBPath = "";
}

error_code DBHandler::create(const string& DBName_)
{
	string path = "D:\\HDSDataBase\\" + DBName_;
	int code = _mkdir(path.c_str());
	if (code == -1)
	{
		return _DATABASE_EXIST;
	}
	_DBName = DBName_;
	_DBPath = "D:\\HDSDataBase\\" + _DBName;
	_dbMeta = DBMeta(_DBPath + "\\meta.json");
	return _TRUE;
}

error_code DBHandler::createTable(const string& tableName_, const vector<string>& columnName_, 
						          const string& primaryKey_ )
{
	int code = _tableHandler.create(_DBPath + "\\" + tableName_, tableName_, primaryKey_, 
		                            columnName_, columnName_.size());
	if (code == _TABLE_EXIST)
	{
		return _TABLE_EXIST;
	}
	_tableName.push_back(tableName_);
	_dbMeta.save(_tableName, _DBPath);
	return _TRUE;
}

error_code DBHandler::createTable(const string& tableName_, const vector<string>& columnName_)
{
	int code = _tableHandler.create(_DBPath + "\\" + tableName_, tableName_, columnName_[0],
		columnName_, columnName_.size());
	/// std::cout << "createTable true" << std::endl; // 正确
	if (code == _TABLE_EXIST)
	{
		return _TABLE_EXIST;
	}
	string _primaryKey = columnName_[0];
	_tableName.push_back(tableName_);
	_dbMeta.save(_tableName, _DBPath);
	// json testJson = _dbMeta.load();
	// std::cout << testJson << std::endl; //正确
	return _TRUE;

}

error_code DBHandler::open(const string& DBName_)
{
	DBMeta dbMeta;
	_DBName = DBName_;
	_DBPath = "D:\\HDSDataBase\\" + _DBName;
	// std::cout << "DBPATH " << _DBPath << std::endl;//正确
	int code = dbMeta.open(_DBPath + "\\meta.json");
	// std::cout << "dbmate: "<<dbMeta.load() << std::endl;
	if (code == _NOT_EXIST_DATABASE)
	{
		return _NOT_EXIST_DATABASE;
	}
	_dbMeta = DBMeta(_DBPath + "\\meta.json");

	// 加载_DBMeta的数据到_tableName中
	json metaJson = _dbMeta.load();
	for (json::iterator it = metaJson.begin(); it != metaJson.end(); ++it)
	{
		int i = 0;
		for (i = 0; i < _tableName.size(); i++)
		{
			if (_tableName[i] == it.key())
				break;
		}
		if(i >= _tableName.size())
			_tableName.push_back(it.key());
	}
	return _TRUE;
}

TableHandler DBHandler::openTable(const string& tableName_)
{
	/// std::cout << "DBPATH " << _DBPath << std::endl;
	_tableHandler.open(_DBPath + "\\" + tableName_, tableName_); 
	// std::cout << "openTable true" << std::endl;  // 调试正确
	return _tableHandler;
}

error_code DBHandler::closeTable(const string& tableName_)
{
	_tableHandler.open(_DBPath + "\\" + tableName_, tableName_);
	_tableHandler.close();
	return _TRUE;
}

error_code DBHandler::drop()
{
	json j = _dbMeta.load();
	string path;
	for (int i = 0; i < j.size(); i++)
	{
		path = j[i];
		_rmdir(path.c_str());
	}
	_rmdir(_DBPath.c_str());
	return _TRUE;
}

error_code DBHandler::dropTable(const string& tableName_)
{
	_tableHandler.open(_DBPath + "\\" + tableName_, tableName_);
	_tableHandler.drop();
	return _TRUE;
}

