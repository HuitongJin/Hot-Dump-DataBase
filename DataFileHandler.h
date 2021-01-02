#pragma once 
// auther: Jackson Kim

/********************************************************************************
* ����һ�����ݹ����࣬ʹ�ö������ļ��洢ĳ�����е���ϸ���ݡ�                    *
* �������������Խ������²�����                                                *
* 1. bool isOpen() �жϴ�����ݵĶ������ļ��Ƿ�򿪡�                           *
* 2. error_code openDataFile(const string&, unsigned int)  ��һ���������ļ��� *
*    ������������½���                                                         *
* 3. error_code openDataFile(const string&) ��һ���Ѿ����ڵĶ������ļ���      *
* 4. error_code append(int*), ����һ��һά�������飬�����ڶ������ļ���ĩβ׷��  *
*    ���ݡ�                                                                     *
* 5. int* read_line(unsigned int) ��ĳһ�п�ʼ����ȡһ�����ݣ�����һά�������顣*
* 6. error_code close() �ر��Ѿ��򿪵Ķ������ļ���                              *
*********************************************************************************/

// ��Ҫ�ر�ע����ǣ���������������Ķ������ļ������������;�Ϊint���͡�

#ifndef _HDSDATABASE_DATAFILEHANDLER_H
#define _HDSDATABASE_DATAFILEHANDLER_H

#include <fstream>
#include <iostream>

using std::string;
using std::fstream;
using std::ios_base;
typedef int error_code;

// ����궨�������
#define _ERROR_FORMAL_PARAMETER  1    // ��������
#define _OPEN_FILE_FAILED        4    // �ļ���ʧ��
#define _NOT_OPEN_FILE           5    // δ���ļ�
#define _WRITE_FILE_FAILED       3    // �ļ�д�����
#define _OUT_LIMITED             2    // ��ĳ�п�ʼ��ȡʱ�������������
#define _READ_FILE_FAILED        6    // �ļ���ȡʧ��
#define _CLOSE_FILE_FAILED       7    // �ļ��ر�ʧ��
#define _TRUE                    0    // ��ȷ����

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
        string             _dataFilePath;  // �������ļ���·��

        fstream            _dataFile;      // ʵ����fstream����

        unsigned int       _sizeof_Line;   // �������
};

#endif // _HDSDATABASE_DATAFILEHANDLER_


