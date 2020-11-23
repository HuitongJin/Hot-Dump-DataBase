#pragma once

// auther: Jackson Kim

/********************************************************************************
* 这是一个表管理类，可以使用这个类对该表的详细数据和元数据进行管理。            *
* 在这个类中你可以进行以下操作：                                                *
* 1. error_code create(const string&, const string&,const string&,              *
*    const vector<string>&,unsigned int) 用于创建一个新的表。                   *
* 2. error_code open(const string&, const string&) 根据路径和表名打开一个表，   *
*    不存在则创建。                                                             *
* 3. json* read(int offSet_) 读取表中某一行数据，返回一个json数组。             *
* 4. error_code write(const json& infoJson_) 在表的末尾追加一行数据。           *
* 5. json get_meta() 获取这个表对应的元数据。                                   *
* 6. error_code close() 将这个表关闭。                                          *
* 7. error_code  drop() 将这个表删除。                                          *
*********************************************************************************/

#include <iostream>
#include <direct.h>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>
#include "DataFileHandler.h"
#include "TableMeta.h"

#ifndef _HDSDATABASE_TABLEHANDLER_H
#define _HDSDATABASE_TABLEHANDLER_H

using std::string;
using std::vector;
using json = nlohmann::json;
typedef int error_code;

// 这里宏定义错误码:
#define _OPEN_FILE_FAILED         4    // 打开文件失败
#define _TABLE_EXIST              9    // 表已经存在，无法创建
#define _CREATE_TABLEMETA_FAILED  11   // 创建元数据失败
#define _ERROR_FORMAL_PARAMETER   1    // 参数出错
#define _CLOSE_FILE_FAILED        7    // 文件关闭出错
#define _DELETE_TABLE_ERROR       12   // 删除表失败

class TableHandler 
{
    public:
        TableHandler();

        TableHandler(const TableHandler& _tableHandler);

        void operator = (const TableHandler& tableHandler_);

        error_code create(const string& tableFilePath_, const string& tableName_,
                          const string& primaryKey_, const vector<string>& columnName_,
                          unsigned int sizeof_column_);

        error_code open(const string& tableFilePath_, const string& tableName_);

        json* read(int offSet_);

        error_code write(const json& infoJson_);

        json get_meta();

        error_code close();

        error_code  drop();

    private:
        string           _tableFilePath;       // 表对应的文件夹路径
        
        string           _tableName;           // 表名
        
        unsigned int     _sizeof_column = 0;   // 列的数量

        DataFileHandler  _dataFileHandler;     // 表对应的二进制数据

        TableMeta        _tableMeta;           // 表对应的元数据

};
#endif // !_HDSDATABASE_TABLEHANDLER_H

