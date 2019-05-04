#ifndef SRC_IRC_SERVER_H
#define SRC_IRC_SERVER_H

#pragma once
#include<map>
#include"irc_error.h"
#include"irc_response.h"
#include"irc_request.h"
#include"irc_channel.h"
#include"irc_user.h"

namespace irc {
    class Server {
    private:
        static Server * _server;
    public:
        std::string host;
        std::string port;
    public:
        std::map<std::string, irc::Channel> ChannelMap;
        std::map<std::string, irc::User> UserMap;    
    public:
        irc::User ReadUser(std::string name);
        irc::User ReadChannel(std::string name);
    public:
        void RunServe(int port); // 运行服务
        static Server* GetInstance();
    };
}
#endif