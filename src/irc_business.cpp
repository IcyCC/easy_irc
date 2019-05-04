//
// Created by 苏畅 on 2019/5/4.
//

#include "irc_business.h"

void irc::business::Login(irc::User &user){

    auto req = user.IRCRead();
    if(req.op == irc::IRC_REQUEST_OP::NICK) {

    }
}