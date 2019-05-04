#include "irc_channel.h"

namespace irc {
    irc::ERROR_NO Channel::IRCBroadcast(irc::IRCResponse &resp)
    {
        for(auto user : users) {
            user->IRCPushMessage(resp);
        }
        return SUCCESS;
    }

    irc::ERROR_NO Channel::IRCJoin(irc::User &user)
    {
        this->users.push_back(&user);
    }

    irc::ERROR_NO Channel::IRCQuit(irc::User &user)
    {
        this->users.remove(&user);
    }
}