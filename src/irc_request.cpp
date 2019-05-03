#include"irc_request.h"
#include"util.h"

irc::IRCRequest::IRCRequest(std::string& _raw) {
    raw = _raw;
    auto msg = SpliteString(raw, " ");
    if (msg.size() < 1) {
        throw "错误的消息";
    }
    
}