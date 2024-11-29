#include <utils/read_file.h>
// ifstream -> string -> istringstream
// 读取文件中的一行文本，将其分割成单词，并将这些单词存储在arg中
bool monitor::ReadFile::readLine(vector<string>* arg) {
    string line;
    // ifs_ ：表示一个输入流，例如 cin。
    // line ：string类型的引用，用来存储输入流中的流信息。

    getline(ifs_, line);
    // eof()函数来判断文件是否为空或者是否读到文件结尾
    if (ifs_.eof() || line.empty()) {
        return false;
    }

    istringstream line_ss(line);
    while (!line_ss.eof()) {
        string word;
        line_ss >> word;
        arg->push_back(word);
    }
    return true;
}