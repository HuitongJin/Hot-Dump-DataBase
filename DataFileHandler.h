#pragma once 
// auther: Jackson Kim

/********************************************************************************
* 这是一个数据管理类，使用二进制文件存储某个表中的详细数据。                    *
* 在这个类中你可以进行以下操作：                                                *
* 1. bool isOpen() 判断存放数据的二进制文件是否打开。                           *
* 2. error_code openDataFile(const string&, unsigned int)  打开一个二进制文件， *
*    如果不存在则新建。                                                         *
* 3. error_code openDataFile(const string&) 打开一个已经存在的二进制文件。      *
* 4. error_code append(int*), 传入一个一维整型数组，用于在二进制文件的末尾追加  *
*    数据。                                                                     *
* 5. int* read_line(unsigned int) 从某一行开始，读取一行数据，返回一维整型数组。*
* 6. error_code close() 关闭已经打开的二进制文件。                              *
*********************************************************************************/

// 需要特别注意的是：这个类中所操作的二进制文件，其数据类型均为int整型。

#ifndef _HDSDATABASE_DATAFILEHANDLER_H
#define _HDSDATABASE_DATAFILEHANDLER_H

#include <fstream>
#include <iostream>

using std::string;
using std::fstream;
using std::ios_base;
typedef int error_code;

// 这里宏定义错误码
#define _ERROR_FORMAL_PARAMETER  1    // 参数错误
#define _OPEN_FILE_FAILED        4    // 文件打开失败
#define _NOT_OPEN_FILE           5    // 未打开文件
#define _WRITE_FILE_FAILED       3    // 文件写入出错
#define _OUT_LIMITED             2    // 从某行开始读取时，传入参数超限
#define _READ_FILE_FAILED        6    // 文件读取失败
#define _CLOSE_FILE_FAILED       7    // 文件关闭失败
#define _TRUE                    0    // 正确操作

class DataFileHandler
{
    public:
        ~DataFileHandler();

        bool isOpen();

        error_code openDataFile(const string& dataFilePath_, int sizeof_Line_);

        error_code openDataFile(const string& dataFilePath_);

        error_code append(int* data_array, int size);

        int* read_line(int begin);

        error_code close();

        void clear();

    private:
        string             _dataFilePath;  // 二进制文件的路径

        fstream            _dataFile;      // 实例化fstream对象

        unsigned int       _sizeof_Line;   // 表的列数
};

#endif // _HDSDATABASE_DATAFILEHANDLER_


