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
            SocketCommunicator SC(this->socket);
            SC.writen(msg.ToString());
        } else {
            this->mesgQueue.push(msg);
        }
        return SUCCESS;
    }

    irc::IRCRequest User::IRCRead()
    {
        SocketCommunicator SC(this->socket);
        std::string buffer;
        SC.getLine(buffer);
        return irc::IRCRequest(buffer);
    }
}
