#pragma once

#include<string>
#include <vector>
#include "irc_user.h"

namespace irc {
    namespace RESP_CODE {
        extern const std::string RPL_WELCOME;
        extern const std::string RPL_YOURHOST;
        extern const std::string RPL_CREATED;
        extern const std::string RPL_MYINFO;

        extern const std::string RPL_LUSERCLIENT;
        extern const std::string RPL_LUSEROP;
        extern const std::string RPL_LUSERUNKNOWN;
        extern const std::string RPL_LUSERCHANNELS;
        extern const std::string RPL_LUSERME;

        extern const std::string RPL_AWAY;
        extern const std::string RPL_UNAWAY;
        extern const std::string RPL_NOWAWAY;

        extern const std::string RPL_WHOISUSER;
        extern const std::string RPL_WHOISSERVER;
        extern const std::string RPL_WHOISOPERATOR;
        extern const std::string RPL_WHOISIDLE;
        extern const std::string RPL_ENDOFWHOIS;
        extern const std::string RPL_WHOISCHANNELS;

        extern const std::string RPL_WHOREPLY;
        extern const std::string RPL_ENDOFWHO;

        extern const std::string RPL_LIST;
        extern const std::string RPL_LISTEND;

        extern const std::string RPL_CHANNELMODEIS;

        extern const std::string RPL_NOTOPIC;
        extern const std::string RPL_TOPIC;

        extern const std::string RPL_NAMREPLY;
        extern const std::string RPL_ENDOFNAMES;

        extern const std::string RPL_MOTDSTART;
        extern const std::string RPL_MOTD;
        extern const std::string RPL_ENDOFMOTD;

        extern const std::string RPL_YOUREOPER;

        extern const std::string ERR_NOSUCHNICK;
        extern const std::string ERR_NOSUCHCHANNEL;
        extern const std::string ERR_CANNOTSENDTOCHAN;
        extern const std::string ERR_NORECIPIENT;
        extern const std::string ERR_NOTEXTTOSEND;
        extern const std::string ERR_UNKNOWNCOMMAND;
        extern const std::string ERR_NOMOTD;
        extern const std::string ERR_NONICKNAMEGIVEN;
        extern const std::string ERR_NICKNAMEINUSE;
        extern const std::string ERR_USERNOTINCHANNEL;
        extern const std::string ERR_NOTONCHANNEL;
        extern const std::string ERR_NOTREGISTERED;
        extern const std::string ERR_NEEDMOREPARAMS;
        extern const std::string ERR_ALREADYREGISTRED;
        extern const std::string ERR_PASSWDMISMATCH;
        extern const std::string ERR_UNKNOWNMODE;
        extern const std::string ERR_CHANOPRIVSNEEDED;
        extern const std::string ERR_UMODEUNKNOWNFLAG;
        extern const std::string ERR_USERSDONTMATCH;
    }

    class IRCResponse {
    public:
        std::string src;
        std::string code;
        std::vector<std::string> cmds;
    public:
        IRCResponse(std::string& _host, std::string& _port, const std::string& _code,  std::vector<std::string>& _cmds) {
            src = ":"+ _host +":"+ _port;
            code = _code;
            for (auto &i : _cmds){
                cmds.push_back(i);
            }
        }
    public:
        std::string ToString();
    };

    class IRCUserResponse :public IRCResponse {
    public:
        irc::User *sender;
    public:
        IRCUserResponse(std::string& _host, std::string& _port, irc::User * _user, const std::string& _code,  std::vector<std::string>& _cmds) :IRCResponse(_host, _port, _code, _cmds){
            sender = _user;
        }
        std::string ToString();
    };
};
