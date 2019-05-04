//
// Created by 苏畅 on 2019/5/4.
//

#include "irc_business.h"
void irc::business::MainLogic(irc::User &user){
   Login(user);
   while(true) {
       auto req = user.IRCRead();
       switch(req.op){
           case irc::IRC_REQUEST_OP::QUIT:
               break;
       }
   }
}

void irc::business::Login(irc::User &user){

    auto server = irc::Server::GetInstance();
    auto req = user.IRCRead();
    if(req.op == irc::IRC_REQUEST_OP::NICK) {
        auto u = server->ReadUser(user.nickName);
        if (u == NULL ){
            user.nickName = req.cmds[1];            
        }
    }
    // 检查nikc
    req = user.IRCRead();
    if (req.op == irc::IRC_REQUEST_OP::USER) {
        user.userName = req.cmds[1];
    }

    auto args =  std::vector<std::string>("amy", 
        "Welcome to the Internet Relay Network borja!borja@polaris.cs.uchicago.edu"
        );

    auto resp = irc::IRCResponse(
        server->host,
        server->port,
        irc::RESP_CODE::RPL_WELCOME,
        args 
    );

    user.IRCPushMessage(resp);
}