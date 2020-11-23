#include "DataFileHandler.h"

DataFileHandler::~DataFileHandler()
{
	if (this->isOpen()) 
	{
		_dataFile.close();
	}
	_dataFilePath = "";
}

bool DataFileHandler::isOpen()
{
	if (_dataFile.is_open())
	{
		return true;
	}

	return false;
}

/*�Զ�������ʽ���ļ��������ж�д������д�������ļ�ĩβ׷��*/
error_code DataFileHandler::openDataFile(const string& dataFilePath_, unsigned int sizeof_Line_)
{
	_dataFilePath = dataFilePath_;
	_sizeof_Line  = sizeof_Line_;
	_dataFile.open(_dataFilePath, ios_base::out | ios_base::binary | ios_base::app);
	if (!_dataFile.is_open())
	{
		return _OPEN_FILE_FAILED;
	}
	return _TRUE;
}

/*���غ�����ֻ�贫��·�������ر�ע�⣺�ļ�Ӧ������*/
error_code DataFileHandler::openDataFile(const string& dataFilePath_)
{
	_dataFilePath = dataFilePath_;
	_dataFile.open(_dataFilePath, ios_base::in | ios_base::out | ios_base::binary | ios_base::app);
	if (!_dataFile.is_open())
	{
		return _OPEN_FILE_FAILED;
	}
	return _TRUE;
}

error_code DataFileHandler::append(int* data_array)
{
	_dataFile.open(_dataFilePath, ios_base::out | ios_base::binary | ios_base::app);
	if ((sizeof(*data_array)/data_array[0]) != _sizeof_Line)
	{
		return _ERROR_FORMAL_PARAMETER;
	}
	_dataFile.seekg(0, ios_base::end);   // �ƶ�seekgָ�룬׼��write.
	_dataFile.write((char*)data_array, sizeof(data_array));
	return _TRUE;
}

int* DataFileHandler::read_line(unsigned int begin) 
{
	_dataFile.open(_dataFilePath, ios_base::in | ios_base::binary | ios_base::app);
	unsigned int offSet_ = unsigned int(begin * _sizeof_Line * sizeof(int));
	_dataFile.seekp(offSet_, ios_base::beg);  // �ƶ�seekpָ�룬׼��read.
	int* data_array = new int[_sizeof_Line];
	_dataFile.read((char*)&data_array, sizeof(data_array));
	return data_array;
}

error_code DataFileHandler::close()
{
	_dataFile.close();
	if (_dataFile.is_open())
		return _CLOSE_FILE_FAILED;
	return _TRUE;
}