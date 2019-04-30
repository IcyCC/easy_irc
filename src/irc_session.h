#pragma once
#include<string>
#include"irc_error.h"
#include"irc_response.h"
#include"irc_request.h"

namespace irc {
    enum USER_STATE {
        IDLE,
        P2P,
        CHANNEL
    };
    class UserSession {
    public:
        USER_STATE state;
        std::string chatName;
    };
}