#pragma once
#include<string>
#include"irc_error.h"
#include"irc_response.h"
#include"irc_request.h"
#include"irc_user.h"

namespace irc {
    extern const std::string ANONYMOUS;
    enum USER_SESSION_STATE {
        IDLE,
        P2P,
        CHANNEL
    };
    
    class UserSession {
    public:
        USER_SESSION_STATE state;
        std::string chatName;

        UserSession():state(IDLE), chatName(ANONYMOUS){}
        UserSession(USER_SESSION_STATE _state, std::string _chatName): state(_state), chatName(_chatName) {};
        ~UserSession() = default;
    };
}