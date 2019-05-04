//
// Created by 苏畅 on 2019/5/4.
//

#pragma once


#include "irc_user.h"
#include "irc_request.h"
#include "irc_server.h"

namespace irc {
    namespace business {
        void MainLogic(irc::User &user);
        void Login(irc::User &user);
    }
}
