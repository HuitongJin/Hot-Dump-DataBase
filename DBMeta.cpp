#include "DBMeta.h"

DBMeta::DBMeta(){}

DBMeta::DBMeta(const string& DBMetaFilePath_)
{
	_DBMetaFilePath = DBMetaFilePath_;
}

DBMeta::~DBMeta()
{
	_DBMetaFilePath = "";
}

error_code DBMeta::save(const vector<string>& tableName_, const string& FilePath_)
{
	_DBMetaFilePath = FilePath_ + "//meta.json";

	json newMetaJson;

	for (int i = 0; i < tableName_.size(); i++)
	{
		newMetaJson[tableName_[i]] = FilePath_ + "\\" + tableName_[i];
	}

	_metaJson = newMetaJson;

	std::fstream _metaFile(_DBMetaFilePath, ios_base::out | ios_base::trunc);
	if (_metaFile.is_open())
	{
		_metaFile << _metaJson;
		_metaFile.close();
		return _TRUE;
	}
	return _CREATE_DATABASEMETA_FAILED;
}

error_code DBMeta::open(const string& DBMetaFilePath_)
{
	_DBMetaFilePath = DBMetaFilePath_;
	std::fstream _metaFile;
	_metaFile.open(_DBMetaFilePath, ios_base::in);
	if (!_metaFile.is_open())
	{
		return _NOT_EXIST_DATABASE;
	}
	json newMetaJson = this->load();
	_metaJson = newMetaJson;
	return _TRUE;
}

json DBMeta::load()
{
	json metaJson_;
	std::ifstream _metaFile(_DBMetaFilePath);
	// std::cout << "_DBMetaFilePath " << _DBMetaFilePath << std::endl;
	if (_metaFile.is_open())
	{
		_metaFile >> metaJson_;
		_metaFile.close();
	}
	_metaJson = metaJson_;
	return metaJson_;
}