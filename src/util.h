// 此处存放工具
#ifndef SRC_IRC_UTIL_H
#define SRC_IRC_UTIL_H
//
// Created by 苏畅 on 2019/1/7.
// 常用的工具函数
//

#include <string>
#include <map>
#include <list>
#include <sstream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<stdio.h>

#define MAX_PATH_LEN 100
#define ACCESS(fileName,accessMode) access(fileName,accessMode)
#define MKDIR(path) mkdir(path,755)


inline std::vector<std::string> SpliteString(std::string src, std::string sp) {
    // 分割字符串
    std::string::size_type pos1, pos2=0;
    std::vector<std::string> v;
    pos2 = src.find(sp);
    pos1 = 0;
    while (std::string::npos != pos2) {
        v.push_back(src.substr(pos1, pos2 - pos1));

        pos1 = pos2 + sp.size();
        pos2 = src.find(sp, pos1);
    }
    if (pos1 != src.length()) {
        v.push_back(src.substr(pos1));
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
#endif