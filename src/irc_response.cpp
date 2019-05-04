//
// Created by 苏畅 on 2019/5/4.
//

#include "irc_response.h"

std::string irc::IRCResponse::ToString() {
    std::string res;
    res = res + src + " ";
    res = res + code + " ";
    for (auto &i: cmds ){
        res = res + i + " ";
    }
    return res;
}