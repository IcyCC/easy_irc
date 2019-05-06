#ifndef SRC_IRC_USER_H
#define SRC_IRC_USER_H
#include<string>
#include<unistd.h>
#include<queue>
#include"irc_error.h"
#include"irc_response.h"
#include"irc_request.h"
#include"irc_session.h"
#include"swrappers.hh"

namespace irc {
    extern const std::string ANONYMOUS;
    class User {
    public:
        int socket;
        std::string ircHost;
        std::string ircPort;
        std::string nickName;
        std::string userName;
        bool state;         //false 离线，true在线
        irc::UserSession session;
        std::queue<irc::IRCResponse> mesgQueue;
    public:
        User();
        User(std::string _nickName, std::string _userName, int _socket);
        ~User()=default;
    public:
        irc::ERROR_NO IRCPushMessage(irc::IRCResponse &msg); // 通过套接字给改用户发送消息
        irc::IRCRequest IRCRead(); // 读取请求
        irc::ERROR_NO Login(int _socket);
        irc::ERROR_NO Logout();
        int PushOfflineMessage();
        std::string GetSrc();
    };
}

#endif