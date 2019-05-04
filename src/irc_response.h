#pragma once

#include<string>
#include <vector>

namespace irc {
    namespace RESP_CODE {
        const std::string RPL_WELCOME = "001";
        const std::string ERR_NICKNAMEINUSE = "443";

    }

    class IRCResponse {
    public:
        std::string src;
        std::string code;
        std::vector<std::string> cmds;
    public:
        IRCResponse(std::string& _host, std::string& _port, const std::string& _code,  std::vector<std::string>& _cmds) {
            src = ":"+ _host +":"+ _port;
            code = _code;
            for (auto &i : _cmds){
                cmds.push_back(i);
            }
        }
    public:
        std::string ToString();
    };
};
