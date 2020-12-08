#include "TableMeta.h"

TableMeta::~TableMeta()
{
	_primaryKey = "";
	_metaFilePath = "";
}

error_code TableMeta::save(const vector<string>& columnName_,const string& primaryKey_,
	                       const string& metaFilePath_)
{
	_primaryKey   = primaryKey_;
	_metaFilePath = metaFilePath_;

	json newMetaJson;

	for (int i = 0; i < columnName_.size(); i++)
	{
		newMetaJson[columnName_[i]] = i;
	}

	newMetaJson["primaryKey"] = newMetaJson[primaryKey_];
	_metaJson = newMetaJson;
	std::ofstream _metaFile(_metaFilePath, ios_base::out | ios_base::trunc);
	if (_metaFile.is_open())
	{
		_metaFile << _metaJson;
		_metaFile.close();
		return _TRUE;
	}
	return _CREATE_TABLEMETA_FAILED;
}

error_code TableMeta::open(const string& metaFilePath_)
{
	_metaFilePath = metaFilePath_;
	json newMetaJson = this->load(_metaFilePath);
	_metaJson = newMetaJson;
	_primaryKey = newMetaJson["primaryKey"];
	return _TRUE;
}


json TableMeta::load(const string& filePath_)
{
	json metaJson_;
	std::ifstream _metaFile(filePath_, ios_base::in);
	if (_metaFile.is_open())
	{
		_metaFile >> metaJson_;
		_metaFile.close();
	}
	return metaJson_;
}