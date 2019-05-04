//
// Created by 苏畅 on 2019/5/4.
//

#include "irc_business.h"
void irc::business::MainLogic(irc::User &user){
   
}

void irc::business::Login(irc::User &user){

    auto req = user.IRCRead();
    if(req.op == irc::IRC_REQUEST_OP::NICK) {
        user.nickName = req.cmds[1];
    }
    // 检查nikc

    req = user.IRCRead();
    if (req.op == irc::IRC_REQUEST_OP::USER) {
        user.userName = req.cmds[1];
    }

    user.IRCPushMessage()
}