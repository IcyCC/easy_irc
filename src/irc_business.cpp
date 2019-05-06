//
// Created by 苏畅 on 2019/5/4.
//

#include "irc_business.h"
void irc::business::MainLogic(int socketfd){
    User* user = NULL;
    try {
        user = Login(socketfd);
        while(user == NULL){
            user = Login(socketfd);
            LogC("登录失败");
        }

        while(true) {
            auto req = user->IRCRead();
            switch(req.op){
                case irc::IRC_REQUEST_OP::QUIT:
                    irc::business::Quit(user, req);
                    break;
                case irc::IRC_REQUEST_OP::PRIVMSG:
                    irc::business::Chat(user, req);
                    break;
                case irc::IRC_REQUEST_OP::MOTD:
                    irc::business::Motd(user, req);
                    break;
                case irc::IRC_REQUEST_OP::JOIN:
                    irc::business::JoinChannel(user, req);
                    break;
                case irc::IRC_REQUEST_OP::PART:
                    irc::business::PartChannel(user, req);
                    break;
                case irc::IRC_REQUEST_OP::NAMES:
                    break;
                case irc::IRC_REQUEST_OP::USER:
                    break;
                default:
                    UnknowResp(user, req);
                    break;
            }
        }
    }
    catch (std::exception e)  {
        user->Logout();
    }

}

irc::User* irc::business::Login(int socketfd){

    auto server = irc::Server::GetInstance();
    auto tmp_user = User();
    auto tmp_user_ptr = &tmp_user;
    tmp_user_ptr -> Login(socketfd);
    auto req = tmp_user.IRCRead();
    if(req.op == irc::IRC_REQUEST_OP::NICK) {
        auto nick = req.cmds[1];
        auto u = server->ReadUser(nick);
        if (u == NULL ){
            tmp_user_ptr = new User();
            tmp_user_ptr->Login(socketfd);
            tmp_user_ptr->nickName = req.cmds[1];
            LogC("请求用户名 " + tmp_user_ptr->nickName + " 获取成功");
        } else {
            // 用户名已经被使用
            if (u->state){
                // 别的用户登录中 无法拿到该会话
                auto args = std::vector<std::string>();
                LogC("请求用户名 " + tmp_user_ptr->nickName + " 失败");
                args.push_back(tmp_user_ptr->nickName);
                args.push_back(nick);
                args.push_back(":Nickname is already in use.");

                auto resp = irc::IRCResponse(
                        server->host,
                        server->port,
                        irc::RESP_CODE::RPL_WELCOME,
                        args
                );
                tmp_user.IRCPushMessage(resp);
                return NULL;
            } else {
                //  该账户重新上线
                u->Login(tmp_user.socket);
                tmp_user_ptr = u;
            }
        }
    } else {
        UnknowResp(tmp_user_ptr, req);
        return NULL;
    }

    while (req.op != irc::IRC_REQUEST_OP::USER) {
        req = tmp_user_ptr->IRCRead();
        UnknowResp(tmp_user_ptr, req);
    }

    tmp_user_ptr->userName = req.cmds[1];
    tmp_user_ptr->state = true;
    server->SetUser(tmp_user_ptr->nickName, tmp_user_ptr);
    LogC("请求名字 " + tmp_user_ptr->userName + " 成功");

    auto args = std::vector<std::string>();

    args.push_back(tmp_user_ptr->nickName);
    args.push_back("Welcome to the Internet Relay Network borja!borja@polaris.cs.uchicago.edu");

    auto resp = irc::IRCResponse(
        server->host,
        server->port,
        irc::RESP_CODE::RPL_WELCOME,
        args 
    );

    tmp_user_ptr->IRCPushMessage(resp);
    return tmp_user_ptr;
}

void irc::business::Chat(irc::User *user, irc::IRCRequest &req) {
    auto server = irc::Server::GetInstance();

    auto chat_nick = req.cmds[1];
    auto msg = req.cmds[2];

    if(chat_nick[0] == '#') {
        // 频道聊天

        auto chat_channel = server->ReadChannel(chat_nick);
        if (chat_channel == NULL) {
            UnKnowNickResp(user,req);
        } else {
            // 找到频道
            auto args = std::vector<std::string>();
            args.push_back(chat_channel->name);
            args.push_back(msg);
            auto resp = irc::IRCResponse(
                    server->host,
                    server->port,
                    irc::RESP_CODE::RPL_WELCOME,
                    args
            );
            chat_channel->IRCBroadcast(resp);
        }

    } else {
        // 用户聊天
        auto char_user = server->ReadUser(chat_nick);
        if (char_user == NULL) {
            // 处理没找到user
            UnKnowNickResp(user,req);
            return;
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
            return;
        }
    }

}

void irc::business::JoinChannel(irc::User *user, irc::IRCRequest &req)
{
    auto server = irc::Server::GetInstance();

    auto char_channel_nick = req.cmds[1];

    auto char_channel = server->ReadChannel(char_channel_nick);
    if (char_channel == NULL) {
        char_channel = new irc::Channel();
        char_channel->name = char_channel_nick;
        server->SetChannel(char_channel_nick, char_channel);
    }
    if(std::find(char_channel->users.begin(), char_channel->users.end(), user)==char_channel->users.end())
        char_channel->users.push_back(user);

    std::vector<std::string> args;

    args.push_back(req.cmds[0]);
    args.push_back(req.cmds[1]);
    irc::IRCResponse resp_ack(
        user->ircHost,
        user->ircPort,
        "",
        args
    );
    char_channel->IRCBroadcast(resp_ack);

    args.clear();
    args.push_back(user->nickName);
    args.push_back("=");
    args.push_back(char_channel_nick);
    args.push_back(":");
    for(auto i:char_channel->users) {
        args.push_back(i->nickName);
    }
    irc::IRCResponse resp_names_list(
        user->ircHost,
        user->ircPort,
        irc::RESP_CODE::RPL_NAMREPLY,
        args
    );
    user->IRCPushMessage(resp_names_list);

    args.clear();
    args.push_back(user->nickName);
    args.push_back(char_channel_nick);
    args.push_back(":End of NAMES list");
    irc::IRCResponse resp_end_list(
        user->ircHost,
        user->ircPort,
        irc::RESP_CODE::RPL_ENDOFNAMES,
        args
    );
    user->IRCPushMessage(resp_end_list);
}

void irc::business::UnknowResp(irc::User *user, irc::IRCRequest &req){
    auto server = irc::Server::GetInstance();
    auto args = std::vector<std::string>();
    args.push_back(req.cmds[0]);
    args.push_back(":Unknown command");
    auto resp = irc::IRCResponse(
            server->host,
            server->port,
            irc::RESP_CODE::ERR_UNKNOWNCOMMAND,
            args
    );
    user->IRCPushMessage(resp);
}

void irc::business::UnKnowNickResp(irc::User *user, irc::IRCRequest &req){
    auto server = irc::Server::GetInstance();
    auto args = std::vector<std::string>();
    args.push_back(req.cmds[1]);
    args.push_back(":No such nick/channel");
    auto resp = irc::IRCResponse(
            server->host,
            server->port,
            irc::RESP_CODE::RPL_WELCOME,
            args
    );
    user->IRCPushMessage(resp);
};


void irc::business::Motd(irc::User *user , irc::IRCRequest &req) {
    if(user->PushOfflineMessage() < 0)
        UnknowResp(user, req);
}

void irc::business::PartChannel(irc::User *user, irc::IRCRequest &req)
{
    auto server = irc::Server::GetInstance();

    auto char_channel_nick = req.cmds[1];

    auto char_channel = server->ReadChannel(char_channel_nick);
    if (char_channel == NULL) {
        UnKnowNickResp(user, req);
        return;
    }
    if(std::find(char_channel->users.begin(), char_channel->users.end(), user)==char_channel->users.end())
        return;
    std::vector<std::string> args(req.cmds);
    irc::IRCResponse resp_ack(
        user->ircHost,
        user->ircPort,
        "",
        args
    );
    char_channel->IRCBroadcast(resp_ack);
    char_channel->users.remove(user);
}

void irc::business::Quit(irc::User *user, irc::IRCRequest &req)
{
    user->Logout();
}

void NAMES(irc::User *user, irc::IRCRequest &req) {
    if (req.cmds.size() == 1){
        // 单个参数
        
    } 
}
