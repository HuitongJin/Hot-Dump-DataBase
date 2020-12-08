#include <iostream>
#include <vector>
#include "DBMeta.h"
#include "TableRow.h"
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;

using json = nlohmann::json;

int main()
{
    DBHandler db;
    db.create("TestDB");
    vector<string> ss;
    ss.push_back("id");
    ss.push_back("age");
    ss.push_back("score");
    db.createTable("student", ss);
    TableRow table("TestDB", "student");
    ifstream i("C:\\Users\\Lenovo\\Desktop\\test_all\\test_data_50.json");
    json j_50;
    i >> j_50;
    i.close();
   // cout << j_100 << endl;
    table.write(j_50);
    json* data = table.read(0,20);
    cout << *data << endl;

    //int** data_array = new int* [10];
    //for (int i = 0; i < 10; i++) {
    //    data_array[i] = new int[ss.size()];
    //    for (int j = 0; j < ss.size(); j++) {
    //        data_array[i][j] = 100 * i + j;
    //    }
    //    std::ofstream _dataFile("C:\\Users\\Lenovo\\Desktop\\test_data.dat", ios_base::out | ios_base::app);
    //    _dataFile.seekp(0, ios_base::end);   // 移动seekp指针，准备write.
    //    _dataFile.write((char*)data_array[i], ss.size() * sizeof(int));
    //    // std::cout << "write true" << std::endl;
    //    _dataFile.close();
    //}

    //ifstream _dataFile("C:\\Users\\Lenovo\\Desktop\\test_data.dat", ios_base::out | ios_base::app);
    //_dataFile.seekg(16 * ss.size() * sizeof(int), ios::beg);
    //int* data = new int[10];
    //if (!_dataFile.eof())
    //{
    //    _dataFile.read((char*)data, sizeof(int) * ss.size());
    //    for (int i = 0; i < ss.size(); i++)
    //        cout << data[i] << " ";
    //    cout << endl;
    //}
    //_dataFile.close();
}
