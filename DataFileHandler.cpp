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

/*以二进制形式打开文件，并进行读写操作，写操作在文件末尾追加*/
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

/*重载函数，只需传入路径，但特别注意：文件应当存在*/
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
	_dataFile.seekg(0, ios_base::end);   // 移动seekg指针，准备write.
	_dataFile.write((char*)data_array, sizeof(data_array));
	return _TRUE;
}

int* DataFileHandler::read_line(unsigned int begin) 
{
	_dataFile.open(_dataFilePath, ios_base::in | ios_base::binary | ios_base::app);
	unsigned int offSet_ = unsigned int(begin * _sizeof_Line * sizeof(int));
	_dataFile.seekp(offSet_, ios_base::beg);  // 移动seekp指针，准备read.
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