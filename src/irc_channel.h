#pragma once

#include<string>
#include"irc_error.h"
#include"irc_response.h"
#include"irc_request.h"
#include"irc_user.h"

namespace irc {
    class Channel {
    public:
        irc::ERROR_NO IRCBroadcast(IRCResponse* resp); // 发送消息
        irc::ERROR_NO IRCJoin(irc::User * user);  // 加入频道
        irc::ERROR_NO IRCQuit(irc::User * user); //退出频道
    };
}