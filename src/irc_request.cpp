#include"irc_request.h"
#include"util.h"

irc::IRCRequest::IRCRequest(std::string& _raw) {
    raw = _raw;
    _raw.pop_back();
    _raw.pop_back();
    auto _cmds = SpliteString(_raw, " ");
    if (_cmds.size() < 1) {
        throw "错误的消息";
    }
    if (_cmds[0] == "NICK") {
        op = irc::IRC_REQUEST_OP::NICK;
    } else if(_cmds[0] == "USER") {
        op = irc::IRC_REQUEST_OP::USER;
    } else if (_cmds[0] == "QUIT"){
        op = irc::IRC_REQUEST_OP::QUIT;
    } else if (_cmds[0] == "PRIVMSG") {
        op = irc::IRC_REQUEST_OP::PRIVMSG;
    } else if (_cmds[0] == "NOTICE") {
        op = irc::IRC_REQUEST_OP::NOTICE;
    } else if (_cmds[0] == "PING"){
        op = irc::IRC_REQUEST_OP::PING;
    } else if (_cmds[0] == "PONG") {
        op = irc::IRC_REQUEST_OP::PONG;
    } else if (_cmds[0] == "MOTD") {
        op = irc::IRC_REQUEST_OP::MOTD;
    } else if (_cmds[0] == "LUSERS") {
        op = irc::IRC_REQUEST_OP::LUSERS;
    } else if (_cmds[0] == "WHOIS") {
        op = irc::IRC_REQUEST_OP::WHOIS;
    } else if (_cmds[0] == "JOIN") {
        op = irc::IRC_REQUEST_OP::JOIN;
    }  else if (_cmds[0] == "PART") {
        op = irc::IRC_REQUEST_OP::PART;
    } else if (_cmds[0] == "NAMES"){
        op = irc::IRC_REQUEST_OP::NAMES;
    } 
    else {
        op = irc::IRC_REQUEST_OP::UNKNOW;
    }

    std::string _msg;
    bool _in_msg = false;
    for(auto c: _cmds) {

        if (c[0] == ':'){
            _in_msg = true;
        }

        if (_in_msg ) {
            _msg = _msg + c+" ";
        } else {
            cmds.push_back(c);
        }
    }
    cmds.push_back(_msg);
}

std::string irc::IRCRequest::ToString() {
    return raw;
}