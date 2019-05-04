#ifndef SRC_IRC_SESSION_H
#define SRC_IRC_SESSION_H
#include<string>
#include"irc_error.h"
#include"irc_response.h"
#include"irc_request.h"
#include"irc_user.h"

namespace irc {
    enum USER_STATE {
        IDLE,
        P2P,
        CHANNEL
    };
    
    class UserSession {
    public:
        USER_STATE state;
        std::string chat_name;
    };
}

#endif