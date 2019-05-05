//
// Created by 苏畅 on 2019/5/4.
//

#pragma once


#include "irc_user.h"
#include "irc_request.h"
#include "irc_server.h"
#include <vector>

namespace irc {
    namespace business {
        bool Login(irc::User *user);

        void Chat(irc::User *user, irc::IRCRequest &req);
        void MainLogic(irc::User *user);
        void Motd(irc::User *user , irc::IRCRequest &req);

        // 错误响应
        void UnknowResp(irc::User *user, irc::IRCRequest &req);
        void UnKnowNickResp(irc::User *user, irc::IRCRequest &req);
    }
}
