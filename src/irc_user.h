#ifndef SRC_IRC_USER_H
#define SRC_IRC_USER_H
#include<string>
#include"irc_error.h"
#include"irc_response.h"
#include"irc_request.h"
#include"irc_session.h"

namespace irc {
    class User {
    public:
        irc::UserSession session;
    public:
        irc::ERROR_NO IRCPushMessage(irc::IRCResponse *msg); // 通过套接字给改用户发送消息
        irc::IRCRequest IRCRead(); // 读取请求
    };
}

#endif