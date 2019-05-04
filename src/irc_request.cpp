#include"irc_request.h"
#include"util.h"

irc::IRCRequest::IRCRequest(std::string& _raw) {
    raw = _raw;
    _raw.pop_back();
    _raw.pop_back();
    cmds = SpliteString(_raw, " ");
    if (cmds.size() < 1) {
        throw "错误的消息";
    }
    if (cmds[0] == "NICK") {
        op = irc::IRC_REQUEST_OP::NICK;
    } else if(cmds[0] == "USRE") {
        op = irc::IRC_REQUEST_OP::USER;
    } else if (cmds[0] == "QUIT"){
        op = irc::IRC_REQUEST_OP::QUIT;
    } else if (cmds[0] == "PRIVMSG") {
        op = irc::IRC_REQUEST_OP::PRIVMSG;
    } else if (cmds[0] == "NOTICE") {
        op = irc::IRC_REQUEST_OP::NOTICE;
    } else if (cmds[0] == "PING"){
        op = irc::IRC_REQUEST_OP::PING;
    } else if (cmds[0] == "PONG") {
        op = irc::IRC_REQUEST_OP::PONG;
    } else if (cmds[0] == "MOTD") {
        op = irc::IRC_REQUEST_OP::MOTD;
    } else if (cmds[0] == "LUSERS") {
        op = irc::IRC_REQUEST_OP::LUSERS;
    } else if (cmds[0] == "WHOIS") {
        op = irc::IRC_REQUEST_OP::WHOIS;
    } else if (cmds[0] == "JOIN") {
        op = irc::IRC_REQUEST_OP::JOIN;
    }  else if (cmds[0] == "PART") {
        op = irc::IRC_REQUEST_OP::PART;
    } 
    else {
        op = irc::IRC_REQUEST_OP::UNKNOW;
    }
}

std::string irc::IRCRequest::ToString() {
    return raw;
}