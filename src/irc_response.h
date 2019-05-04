#ifndef SRC_IRC_RESPONSE_H
#define SRC_IRC_RESPONSE_H

#include<string>
#include <vector>
#include "irc_server.h"

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
        IRCResponse(const std::string _code,  std::vector<std::string>& _cmds) {
            auto server = irc::Server::GetInstance();
            src = ":"+ server->host +":"+server->port;
            code = _code;
            for (auto &i : _cmds){
                cmds.push_back(i);
            }
        };
    public:
        std::string ToString();
    };
}
#endif //SRC_IRC_RESPONSE_H
