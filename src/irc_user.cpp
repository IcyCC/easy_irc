#include "irc_user.h"

namespace irc {
    const std::string ANONYMOUS = "*";

    User::User()
    {
        this->nickName = irc::ANONYMOUS;
        this->userName = irc::ANONYMOUS;
        this->state = false;
        this->session.state = IDLE;
        this->socket = -1;
    }

    User::User(std::string _nickName,std::string _userName, int _socket)
    {
        this->nickName = _nickName;
        this->userName = _userName;
        this->socket = _socket;
        this->state = true;
        this->session.state = IDLE;
    }

    irc::ERROR_NO User::Login(int _socket)
    {
        this->socket = _socket;
        this->state = true;
        while(!mesgQueue.empty()) {
            IRCPushMessage(mesgQueue.front());
            mesgQueue.pop();
        }
        return SUCCESS;
    }

    irc::ERROR_NO User::Logout()
    {
        this->socket = -1;
        this->state = false;
        return SUCCESS;
    }

    irc::ERROR_NO User::IRCPushMessage(irc::IRCResponse &msg)
    {
        if(this->state) {
            SWriten(this->socket, msg.ToString());
        } else {
            this->mesgQueue.push(msg);
        }
        return SUCCESS;
    }

    irc::IRCRequest User::IRCRead()
    {
        std::string buffer = SRead(this->socket);
        return irc::IRCRequest(buffer);
    }
}
