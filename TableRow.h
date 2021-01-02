#pragma once

#ifndef _HDSDATABASE_TABLEROW_H
#define _HDSDATABASE_TABLEROW_H

#include <fstream>
#include <vector>
#include <cassert>
#include <map>
#include <nlohmann/json.hpp>
#include "TableHandler.h"
#include "DBHandler.h"
#include "Error.h"

using std::string;
using std::vector;
using json = nlohmann::json;

class TableRow
{
	public:
		TableRow();
		
		TableRow(const string& dataBaseName_, const string& tableName_);

		json* read(int offSet_, int numberOfLine_);

		int write(const json& rowInformation_);

		json* get_column(const string& columnName_);

		void clear();

	private:
		TableHandler _tableHandler;
		Error ERROR;
};
#endif // !_HDSDATABASE_TABLEROW_H
