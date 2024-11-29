#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
namespace monitor {

class ReadFile {
   public:
    // 该构造函数是显式的，而非隐式的
    explicit ReadFile(const string& path) : ifs_(path) {}
    // 析构函数
    ~ReadFile() { ifs_.close(); }

    bool readLine(vector<string>* arg);

   private:
    // 读取文件内容，可以打开并读取文件中的数据
    ifstream ifs_;
};

};  // namespace monitor