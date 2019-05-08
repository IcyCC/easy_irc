#include "irc_user.h"

namespace irc {
    const std::string ANONYMOUS = "*";

    User::User()
    {
        this->nickName = irc::ANONYMOUS;
        this->userName = irc::ANONYMOUS;
        this->realName = irc::ANONYMOUS;
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

        ComboAddress socket_name;
        SGetsockname(socket, socket_name);
        this->ircHost = socket_name.toString();
        this->ircPort = socket_name.portToString();

        return SUCCESS;
    }

    irc::ERROR_NO User::Logout()
    {
        if(socket > 0)
            shutdown(socket, SHUT_RDWR);
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

    std::vector<std::string> irc::User::PushOfflineMessage()
    {   
        std::vector<std::string> msgs;
        std::ifstream inf("motd.txt");
        std::ostringstream tmp;
        tmp << inf.rdbuf();
        std::string tmp_str = tmp.str();
        int pos = 0;
        int pre_pos = 0;
        while((pos = tmp_str.find('\n', pre_pos)) != std::string::npos) {
            msgs.push_back(tmp_str.substr(pre_pos, pos-pre_pos));
            pre_pos = pos = pos+1;
        }
        int n = tmp_str.size();
        if(pre_pos < n - 1) {
            msgs.push_back(tmp_str.substr(pre_pos, tmp_str.size() - pre_pos));
        }
        return msgs;
    }

    std::string irc::User::GetSrc()
    {
        return nickName + "!" + userName + "@" + ircHost;
    }
}
