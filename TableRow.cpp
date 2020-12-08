#include "TableRow.h"

TableRow::TableRow(){}

TableRow::TableRow(const string& dataBaseName_, const string& tableName_)
{
	DBHandler dbHandler;
	dbHandler.open(dataBaseName_);
	_tableHandler = dbHandler.openTable(tableName_);
}

json* TableRow::read(int offSet_, int numberOfLine_)
{
	json* infoJson = new json;
	*infoJson = json::array();
	for (int i = 0; i < numberOfLine_; i++)
	{
		json* newJson = _tableHandler.read(offSet_ + i);
		// std::cout << *newJson << std::endl;
		infoJson->push_back(*newJson);
	}
	return infoJson;
}

int TableRow::write(const json& rowInformation_)
{
	for (int i = 0; i < rowInformation_.size(); i++)
	{
		// std::cout << rowInformation_[i] << std::endl; // 正确
		_tableHandler.write(rowInformation_[i]);
	}
	return 1;
}

// 未完成实现
json* TableRow::get_column(const string& columnName_)
{
	json* infoJson = new json;
	*infoJson = json::array();
	json meta_json = _tableHandler.get_meta();
	int index = meta_json[columnName_];
	return infoJson;
}