//
// Created by 苏畅 on 2019/5/4.
//

#include "irc_response.h"

const std::string irc::RESP_CODE::RPL_WELCOME = "001";
const std::string irc::RESP_CODE::ERR_NICKNAMEINUSE = "433";


std::string irc::IRCResponse::ToString() {
    std::string res;
    res = res + src + " ";
    res = res + code + " ";
    for (auto &i: cmds ){
        res = res + i + " ";
    }
    res  = res + "\r\n";
    return res;
}