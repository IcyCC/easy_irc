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
        void Login(irc::User &user);

        void Chat(irc::User &user, irc::IRCRequest &req);
        void MainLogic(irc::User &user);

    }
}
