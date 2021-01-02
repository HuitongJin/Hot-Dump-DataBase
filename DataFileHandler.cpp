#include "DataFileHandler.h"

DataFileHandler::~DataFileHandler()
{
	if (this->isOpen()) 
	{
		_dataFile.close();
	}
	_sizeof_Line = 0;
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
error_code DataFileHandler::openDataFile(const string& dataFilePath_, int sizeof_Line_)
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

error_code DataFileHandler::append(int* data_array, int size)
{
	std::ofstream _dataFile(_dataFilePath, ios_base::out | ios_base::app);
	if (size != _sizeof_Line)
	{
		std::cout << "parameter formal error" << std::endl;
		return _ERROR_FORMAL_PARAMETER;
	}
	_dataFile.seekp(0, ios_base::end);   // 移动seekp指针，准备write.
	_dataFile.write((char*)data_array,_sizeof_Line*sizeof(int));
	// std::cout << "write true" << std::endl;
	_dataFile.close();
	return _TRUE;
}

int* DataFileHandler::read_line(int begin) 
{
	std::ifstream _dataFile(_dataFilePath, ios_base::in | ios_base::app);
	int offSet_ = int(begin * _sizeof_Line * sizeof(int));
	_dataFile.seekg(offSet_, ios_base::beg);  // 移动seekg指针，准备read.
	int* data_array = new int[_sizeof_Line];
	if (!_dataFile.eof()) {
		_dataFile.read((char*)data_array, _sizeof_Line * sizeof(int));
		return data_array;
	}
	else {
		std::cout << "文件读取越界" << std::endl;
		return data_array;
	}
	/*for (int i = 0; i < _sizeof_Line; i++)
		std::cout << data_array[i] << " ";
	std::cout << std::endl;*/
	
}

error_code DataFileHandler::close()
{
	_dataFile.close();
	if (_dataFile.is_open())
		return _CLOSE_FILE_FAILED;
	return _TRUE;
}

void DataFileHandler::clear()
{
	_dataFile.clear();
}