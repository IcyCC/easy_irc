//
// Created by 苏畅 on 2019/5/4.
//

#pragma once


#include "irc_user.h"
#include "irc_request.h"
#include "irc_server.h"
#include <vector>
#include <algorithm>

namespace irc {
    namespace business {



        void JoinChannel(irc::User *user, irc::IRCRequest &req);
        void PartChannel(irc::User *user , irc::IRCRequest &req);
        void Chat(irc::User *user, irc::IRCRequest &req);
        void Motd(irc::User *user , irc::IRCRequest &req);
        void Quit(irc::User *user , irc::IRCRequest &req);

        // 错误响应
        void UnknowResp(irc::User *user, irc::IRCRequest &req);
        void UnKnowNickResp(irc::User *user, irc::IRCRequest &req);
        User* Login(int socketfd);

        void MainLogic(int socketfd);
    }
}
