// 此处存放工具
#pragma once

//
// Created by 苏畅 on 2019/1/7.
// 常用的工具函数
//

#include <string>
#include <map>
#include <list>
#include <sstream>
#include <vector>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <algorithm>

#define MAX_PATH_LEN 100
#define ACCESS(fileName,accessMode) access(fileName,accessMode)
#define MKDIR(path) mkdir(path,755)

// trim from start 
inline std::string &ltrim(std::string &s) { 
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace)))); 
    return s; 
} 

// trim from end 
inline std::string &rtrim(std::string &s) { 
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end()); 
    return s; 
} 

inline std::string &trim(std::string &s) { 
    return ltrim(rtrim(s)); 
} 

inline std::vector<std::string> SpliteString(std::string src, std::string sp) {
    // 分割字符串
    std::string::size_type pos1, pos2=0;
    std::vector<std::string> v;
    pos2 = src.find(sp);
    pos1 = 0;
    while (std::string::npos != pos2) {
        std::string tmp = src.substr(pos1, pos2 - pos1);

        tmp = trim(tmp);
        if(!tmp.empty())
            v.push_back(trim(tmp));

        pos1 = pos2 + sp.size();
        pos2 = src.find(sp, pos1);
    }
    if (pos1 != src.length()) {
        std::string tmp = src.substr(pos1);
        tmp = trim(tmp);
        if(!tmp.empty())
            v.push_back(trim(tmp));
    }
    return v;
};

inline std::string & ToLower(std::string& s){
    int len=s.size();
    for(int i=0;i<len;i++){
        if(s[i]>='A'&&s[i]<='Z'){
            s[i]+=32;//+32转换为小写
        }
    }
    return s;
}


/*创建类似于a/b/c/d 文件夹*/
inline int CreateDirectory(const std::string directoryPath)
{
    auto temp = SpliteString(directoryPath, "/");
    temp.pop_back();
    std::string tmp_path = "";
    for(auto i : temp){
        tmp_path += i;
        tmp_path +="/";
        auto ok = MKDIR(tmp_path.c_str());
        if (ok == -1){
            perror("MKDIR");
        }
    }
    return 0;
}


inline bool IsBeginWith(std::string s, std::string m) {
    if (m.length() > s.length()) {
        return false;
    }
    for (int i = 0; i < m.length(); i++) {
        if (m[i] != s[i]) {
            return false;
        }
    }
    return true;
}

inline bool IsEndWith(std::string s, std::string m) {
    if (m.length() > s.length()) {
        return false;
    }
    for (int i = 0; i < m.length(); i++) {
        if (m[i] != s[s.length() - m.length() + i]) {
            return false;
        }
    }
    return true;
}


inline void LogC(std::string s){
    std::cout<<s<<std::endl;
}
