//
// Created by kai on 2023/9/2.
//
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

//将char数组分割，并返回分割后的数组，split(原始字符串,分隔符,接收数组)
__attribute__((unused)) vector<string> split(const string &str,const string &pattern)
{
    //const char* convert to char*
    char * str_c = new char[strlen(str.c_str()) + 1];
    strcpy(str_c, str.c_str());
    vector<string> resultVec;
    char* tmpStr = strtok(str_c, pattern.c_str());
    while (tmpStr != nullptr)
    {
        resultVec.push_back(string(tmpStr));
        tmpStr = strtok(nullptr, pattern.c_str());
    }

    delete[] str_c;
    return resultVec;
}

int main() {
    std::vector<string> vec = split("1111111&&&&sdew", "&&&&");
    std::cout << vec[0] << "\n";
    std::cout << vec[1] << "\n";
}
