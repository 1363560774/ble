//
// Created by kai on 2023/9/2.
//
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

//将char数组分割，并返回分割后的数组，split(原始字符串,分隔符,接收数组)
__attribute__((unused)) vector<string> str_split(const string &str,const string &pattern)
{
    //const char* convert to char*
    char * str_c = new char[strlen(str.c_str()) + 1];
    strcpy(str_c, str.c_str());
    vector<string> resultVec;
    char* tmpStr = strtok(str_c, pattern.c_str());
    while (tmpStr != nullptr)
    {
        resultVec.emplace_back(tmpStr);
        tmpStr = strtok(nullptr, pattern.c_str());
    }

    delete[] str_c;
    return resultVec;
}

//int main() {
//    std::vector<string> vec = str_split("1111111&&&&2222", "&&&&");
//    std::cout << vec[0] << "\n";
//    std::cout << vec[1] << "\n";
//    const char* ssid = "ssid";
//    const char* password = "password";
//    uint8_t s_len = strlen(ssid);
//    uint8_t p_len = strlen(password);
//    char result[s_len+p_len];
//    sprintf(result, "%s%s%s", "ssid", "&&&&", "password");
//    printf("%s\n", result);
//
//    string ssid_ = "ssid";
//    string password_ = "password";
//    std::string rxValue = "wifi:ssid&=ssid123|$|password&=password123";
//    if (strstr(rxValue.c_str(), "wifi:ssid&=")) {
//        std::vector<string> wifi = str_split(rxValue, "|$|");
//        std::vector<string> ssid_str = str_split(wifi[0], "&=");
//        std::vector<string> password_str = str_split(wifi[1], "&=");
//        if (ssid_str[1].length() > 1) {
//            ssid_ = ssid_str[1];
//        }
//        if (password_str[1].length() > 7) {
//            password_ = password_str[1];
//        }
//    }
//    printf("%s\n", ssid_.c_str());
//    printf("%s\n", password_.c_str());

//    char str[] = "a,b,c,d";
//    vector<string> resultVec;
//    char a = 0;
//    const char* delimiters = ",";
//    char* token = strtok(str, delimiters);
//    while (token != nullptr) {
////        printf("%s\n", token);
//        resultVec[a] = token;
//        a++;
//        token = strtok(nullptr, delimiters);
//    }
//    printf("%s\n", resultVec.data());
//}
