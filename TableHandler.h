#pragma once

// auther: Jackson Kim

/********************************************************************************
* ����һ��������࣬����ʹ�������Ըñ����ϸ���ݺ�Ԫ���ݽ��й���            *
* �������������Խ������²�����                                                *
* 1. error_code create(const string&, const string&,const string&,              *
*    const vector<string>&,unsigned int) ���ڴ���һ���µı�                   *
* 2. error_code open(const string&, const string&) ����·���ͱ�����һ����   *
*    �������򴴽���                                                             *
* 3. json* read(int offSet_) ��ȡ����ĳһ�����ݣ�����һ��json���顣             *
* 4. error_code write(const json& infoJson_) �ڱ��ĩβ׷��һ�����ݡ�           *
* 5. json get_meta() ��ȡ������Ӧ��Ԫ���ݡ�                                   *
* 6. error_code close() �������رա�                                          *
* 7. error_code  drop() �������ɾ����                                          *
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

// ����궨�������:
#define _OPEN_FILE_FAILED         4    // ���ļ�ʧ��
#define _TABLE_EXIST              9    // ���Ѿ����ڣ��޷�����
#define _CREATE_TABLEMETA_FAILED  11   // ����Ԫ����ʧ��
#define _ERROR_FORMAL_PARAMETER   1    // ��������
#define _CLOSE_FILE_FAILED        7    // �ļ��رճ���
#define _DELETE_TABLE_ERROR       12   // ɾ����ʧ��

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
        string           _tableFilePath;       // ���Ӧ���ļ���·��
        
        string           _tableName;           // ����
        
        unsigned int     _sizeof_column = 0;   // �е�����

        DataFileHandler  _dataFileHandler;     // ���Ӧ�Ķ���������

        TableMeta        _tableMeta;           // ���Ӧ��Ԫ����

};
#endif // !_HDSDATABASE_TABLEHANDLER_H

