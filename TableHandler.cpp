#include "TableHandler.h"

TableHandler::TableHandler(){}

// 拷贝构造函数。
TableHandler::TableHandler(const TableHandler& _tableHandler)
{
    _tableFilePath =  _tableHandler._tableFilePath;
    _tableName     =  _tableHandler._tableName;
    _tableMeta     =  _tableHandler._tableMeta;
    _sizeof_column =  _tableHandler._sizeof_column;
    // std::cout << "拷贝构造函数:" << _sizeof_column << std::endl;
    _dataFileHandler.openDataFile(_tableFilePath + "\\data.dat", _sizeof_column);
}

// 重载=运算符。
void TableHandler::operator = (const TableHandler& _tableHandler)
{
    _tableFilePath = _tableHandler._tableFilePath;
    _tableName     = _tableHandler._tableName;
    _tableMeta     = _tableHandler._tableMeta;
    _sizeof_column = _tableHandler._sizeof_column;
    _dataFileHandler.openDataFile(_tableFilePath + "\\data.dat", _sizeof_column);
}

error_code TableHandler::create(const string& tableFilePath_, const string& tableName_,
                                const string& primaryKey_,    const vector<string>& columnName_,
                                int sizeof_column_)
{
    int code = _mkdir(tableFilePath_.c_str());
    if (code == -1)
    {
        return _TABLE_EXIST;
    }
    
    _tableFilePath = tableFilePath_;
    _tableName = tableName_;
    _sizeof_column = sizeof_column_;
    // std::cout << "create"<<_sizeof_column << std::endl; // 调试正确
    code = _dataFileHandler.openDataFile(_tableFilePath + "\\data.dat", _sizeof_column);
    if (code == _OPEN_FILE_FAILED)
    {
        return _OPEN_FILE_FAILED;
    }
    code = _tableMeta.save(columnName_, primaryKey_, _tableFilePath + "\\dataMeta.json");
    json meta;
    meta = _tableMeta.load(_tableFilePath + "\\dataMeta.json");
   //  std::cout << "create meta " << meta << std::endl; // 正确
    if (code == _CREATE_TABLEMETA_FAILED)
    {
        return _CREATE_TABLEMETA_FAILED;
    }
    return _TRUE;
}

error_code TableHandler::open(const string& tableFilePath_, const string& tableName_)
{
    _tableFilePath = tableFilePath_;
    _tableName = tableName_;
   // std::cout << tableFilePath_ << std::endl;
    json tableMeta_ = _tableMeta.load(tableFilePath_+"\\dataMeta.json");
   // std::cout << tableMeta_ << std::endl;
    // std::cout << true << std::endl;
    _sizeof_column = tableMeta_.size() - 1;
    int code = _dataFileHandler.openDataFile(tableFilePath_ + "\\data.dat", _sizeof_column);
    if (code == _OPEN_FILE_FAILED)
    {
        return _OPEN_FILE_FAILED;
    }
    return _TRUE;
}

json* TableHandler::read(int offSet_)
{
    json* infoJson = new json;
    *infoJson = json::array();
    int* info_array = new int[_sizeof_column];
    info_array = _dataFileHandler.read_line(offSet_);
    for (int i = 0; i < _sizeof_column; i++)
    {
        infoJson->push_back(info_array[i]);
    }
    delete[]info_array;
    return infoJson;
}

error_code TableHandler::write(const json& infoJson_)
{
    int* info_array = new int[_sizeof_column];
    if (infoJson_.size() != _sizeof_column)
    {
        return _ERROR_FORMAL_PARAMETER;
    }
   // std::cout << infoJson_ << std::endl;
    for (int i = 0; i < _sizeof_column; i++)
    {
        if (!infoJson_[i].is_number_integer())
        {
            return _ERROR_FORMAL_PARAMETER;
            std::cout << "_ERROR_FORMAL_PARAMETER" << std::endl;
        }
        info_array[i] = infoJson_[i];
    }
    int code = _dataFileHandler.append(info_array, _sizeof_column);
    if (code == _ERROR_FORMAL_PARAMETER)
    {
        return _ERROR_FORMAL_PARAMETER;
    }
    delete[]info_array;
    return _TRUE;
}

json TableHandler::get_meta()
{
    return _tableMeta.load(_tableFilePath+"\\dataMeta.json");
}

error_code TableHandler::close()
{
    _dataFileHandler.close();
    if (_dataFileHandler.isOpen())
    {
        return _CLOSE_FILE_FAILED;
    }
    return _TRUE;
}

void TableHandler::clear()
{
    _dataFileHandler.clear();
}

error_code  TableHandler::drop()
{
    _dataFileHandler.close();
    int code = _rmdir(_tableFilePath.c_str());
    if (code == -1)
    {
        return _DELETE_TABLE_ERROR;
    }
    return _TRUE;
   /* std::remove((_tableFilePath + "//data.bat").c_str());
    std::remove((_tableFilePath  + "//meta.json").c_str());*/
}