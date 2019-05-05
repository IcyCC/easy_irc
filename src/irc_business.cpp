//
// Created by 苏畅 on 2019/5/4.
//

#include "irc_business.h"
void irc::business::MainLogic(irc::User *user){
   Login(user);
   while(true) {
       auto req = user->IRCRead();
       switch(req.op){
           case irc::IRC_REQUEST_OP::QUIT:
               break;
           case irc::IRC_REQUEST_OP::PRIVMSG:
               irc::business::Chat(user, req);
               break;
           case irc::IRC_REQUEST_OP::JOIN:
               irc::business::JoinChannel(user, req);
           default:
               break;
       }
   }
}

void irc::business::Login(irc::User *user){

    auto server = irc::Server::GetInstance();
    auto req = user->IRCRead();
    if(req.op == irc::IRC_REQUEST_OP::NICK) {
        auto nick = req.cmds[1];
        auto u = server->ReadUser(nick);
        if (u == NULL ){
            user->nickName = req.cmds[1];
            LogC("请求用户名 " + user->nickName + " 获取成功");
        } else {
            // 用户名已经被使用
            auto args = std::vector<std::string>();
            LogC("请求用户名 " + user->nickName + " 失败");
            args.push_back(user->nickName);
            args.push_back(nick);
            args.push_back(":Nickname is already in use.");

            auto resp = irc::IRCResponse(
                    server->host,
                    server->port,
                    irc::RESP_CODE::RPL_WELCOME,
                    args
            );
            user->IRCPushMessage(resp);
            return;
        }
    }

    req = user->IRCRead();
    if (req.op == irc::IRC_REQUEST_OP::USER) {
        user->userName = req.cmds[1];
    }
    user->state = true;
    server->SetUser(user->nickName, user);
    LogC("请求名字 " + user->userName + " 成功");

    auto args = std::vector<std::string>();

    args.push_back(user->nickName);
    args.push_back("Welcome to the Internet Relay Network borja!borja@polaris.cs.uchicago.edu");

    auto resp = irc::IRCResponse(
        server->host,
        server->port,
        irc::RESP_CODE::RPL_WELCOME,
        args 
    );

    user->IRCPushMessage(resp);
}

void irc::business::Chat(irc::User *user, irc::IRCRequest &req) {
    auto server = irc::Server::GetInstance();

    auto char_user_nick = req.cmds[1];
    auto msg = req.cmds[2];

    auto char_user = server->ReadUser(char_user_nick);
    if (char_user == NULL) {
        // 处理没找到user

    } else {
        // 找到user
        auto args = std::vector<std::string>();
        args.push_back(char_user->nickName);
        args.push_back(msg);
        auto resp = irc::IRCResponse(
                server->host,
                server->port,
                irc::RESP_CODE::RPL_WELCOME,
                args
        );
        char_user->IRCPushMessage(resp);
    }
}

void JoinChannel(irc::User *user, irc::IRCRequest &req)
{
    auto server = irc::Server::GetInstance();

    auto char_channel_nick = req.cmds[1];

    auto char_channel = server->ReadChannel(char_channel_nick);
    if (char_channel == NULL) {
        auto channel = new irc::Channel();
        channel->name = char_channel_nick;
        channel->users.push_back(user);
        server->SetChannel(char_channel_nick, channel);
        

    } else {
        auto args = std::vector<std::string>();
        args.push_back(char_user->nickName);
        args.push_back(msg);
        auto resp = irc::IRCResponse(
                server->host,
                server->port,
                irc::RESP_CODE::RPL_WELCOME,
                args
        );
        char_user->IRCPushMessage(resp);
    }
}
