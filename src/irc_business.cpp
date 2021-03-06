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
            // LogC("登录失败");
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
                    irc::business::NAMES(user, req);
                    break;
                case irc::IRC_REQUEST_OP::NOTICE:
                    irc::business::Notice(user, req);
                    break;
                case irc::IRC_REQUEST_OP::PING:
                    irc::business::Ping(user, req);
                    break;
                case irc::IRC_REQUEST_OP::LUSERS:
                    irc::business::Lusers(user, req);
                    break;
                case irc::IRC_REQUEST_OP::WHOIS:
                    irc::business::Whois(user, req);
                    break;
                case irc::IRC_REQUEST_OP::PONG:
                    break;
                case irc::IRC_REQUEST_OP::USER:
                    break;
                case irc::IRC_REQUEST_OP::UNKNOW:
                    UnknowResp(user, req);
                    break;
                default:
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
        GET_NICK:
        if(req.cmds.size() == 1) {
            auto args = std::vector<std::string>();
            auto resp = irc::IRCResponse(
                server->GetSrc(),
                irc::RESP_CODE::ERR_NONICKNAMEGIVEN,
                tmp_user_ptr->nickName,
                args 
            );
            tmp_user_ptr->IRCPushMessage(resp);
            return NULL;
        }
        auto nick = req.cmds[1];
        auto u = server->ReadUser(nick);
        if (u == NULL ){
            tmp_user_ptr = new User();
            tmp_user_ptr->Login(socketfd);
            tmp_user_ptr->nickName = nick;
            // LogC("请求用户名 " + tmp_user_ptr->nickName + " 获取成功");
        } else {
            // 用户名已经被使用
            // LogC("用户名 " + nick + " 已经被使用");
            auto args0 = std::vector<std::string>();
            args0.clear();
            args0.push_back(req.cmds[1]);
            auto resp = irc::IRCResponse(
                    server->GetSrc(),
                    irc::RESP_CODE::ERR_NICKNAMEINUSE,
                    tmp_user.nickName,
                    args0
            );
            tmp_user.IRCPushMessage(resp);
            if (u->state){
                // 别的用户登录中 无法拿到该会话
                auto args = std::vector<std::string>();
                // LogC("请求用户名 " + tmp_user_ptr->nickName + " 失败");
                args.push_back(tmp_user_ptr->nickName);
                args.push_back(nick);
                auto resp = irc::IRCResponse(
                        server->GetSrc(),
                        irc::RESP_CODE::ERR_ALREADYREGISTRED,
                        tmp_user_ptr->nickName,
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
        req = tmp_user.IRCRead();
        while (req.op != irc::IRC_REQUEST_OP::USER || req.cmds.size() < 5) {
            if(req.op == irc::IRC_REQUEST_OP::USER) {
                auto args = std::vector<std::string>();
                args.push_back(req.cmds[0]);
                auto resp = irc::IRCResponse(
                    server->GetSrc(),
                    irc::RESP_CODE::ERR_NEEDMOREPARAMS,
                    tmp_user_ptr->nickName,
                    args 
                );
                tmp_user_ptr->IRCPushMessage(resp);
                req = tmp_user_ptr->IRCRead();
            } else if(req.op == irc::IRC_REQUEST_OP::NICK) {
                tmp_user_ptr->state = false;
                goto GET_NICK;
            } else if(req.op == irc::IRC_REQUEST_OP::UNKNOW || req.op == irc::IRC_REQUEST_OP::OTHER) {
                continue;
            } else {
                auto args = std::vector<std::string>();

                auto resp = irc::IRCResponse(
                    server->GetSrc(),
                    irc::RESP_CODE::ERR_NOTREGISTERED,
                    tmp_user_ptr->nickName,
                    args 
                );
                tmp_user_ptr->IRCPushMessage(resp);
                req = tmp_user_ptr->IRCRead();
            }   
        }

        tmp_user_ptr->userName = req.cmds[1];
        tmp_user_ptr->state = true;
        server->SetUser(tmp_user_ptr->nickName, tmp_user_ptr);
        server->ReduceUnknown();
        // LogC("请求名字 " + tmp_user_ptr->userName + " 成功");
    } else if(req.op == irc::IRC_REQUEST_OP::USER) {
        if(req.cmds.size() < 5) {
            auto args = std::vector<std::string>();
            args.push_back(req.cmds[0]);
            auto resp = irc::IRCResponse(
                server->GetSrc(),
                irc::RESP_CODE::ERR_NEEDMOREPARAMS,
                tmp_user_ptr->nickName,
                args 
            );
            tmp_user_ptr->IRCPushMessage(resp);
            return NULL;
        }
        auto user_name = req.cmds[1];
        req = tmp_user_ptr->IRCRead();
        while (req.op != irc::IRC_REQUEST_OP::NICK || req.cmds.size() < 2) {
            if(req.op == irc::IRC_REQUEST_OP::USER)
                user_name = req.cmds[1];
            else if(req.op == irc::IRC_REQUEST_OP::NICK) {
                auto args = std::vector<std::string>();
                auto resp = irc::IRCResponse(
                    server->GetSrc(),
                    irc::RESP_CODE::ERR_NONICKNAMEGIVEN,
                    tmp_user_ptr->nickName,
                    args 
                );
                tmp_user_ptr->IRCPushMessage(resp);
            } else if(req.op == irc::IRC_REQUEST_OP::UNKNOW || req.op == irc::IRC_REQUEST_OP::OTHER){
                
            } else {
                auto args = std::vector<std::string>();

                auto resp = irc::IRCResponse(
                    server->GetSrc(),
                    irc::RESP_CODE::ERR_NOTREGISTERED,
                    tmp_user_ptr->nickName,
                    args 
                );
                tmp_user_ptr->IRCPushMessage(resp);
            }
            req = tmp_user_ptr->IRCRead();
        }
        auto nick = req.cmds[1];
        auto u = server->ReadUser(nick);
        if (u == NULL ){
            tmp_user_ptr = new User();
            tmp_user_ptr->Login(socketfd);
            tmp_user_ptr->nickName = nick;
            tmp_user_ptr->userName = user_name;
            // LogC("请求用户名 " + tmp_user_ptr->nickName + " 获取成功");
        } else {
            // 用户名已经被使用
            if (u->state){
                // 别的用户登录中 无法拿到该会话
                auto args = std::vector<std::string>();
                // LogC("请求用户名 " + tmp_user_ptr->nickName + " 失败");
                args.push_back(tmp_user_ptr->nickName);
                args.push_back(nick);
                auto resp = irc::IRCResponse(
                        server->GetSrc(),
                        irc::RESP_CODE::ERR_ALREADYREGISTRED,
                        tmp_user_ptr->nickName,
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
        server->SetUser(tmp_user_ptr->nickName, tmp_user_ptr);
        server->ReduceUnknown();
    } else if(req.op == irc::IRC_REQUEST_OP::UNKNOW || req.op == irc::IRC_REQUEST_OP::OTHER) {
        return NULL;
    } else {
        auto args = std::vector<std::string>();

        auto resp = irc::IRCResponse(
            server->GetSrc(),
            irc::RESP_CODE::ERR_NOTREGISTERED,
            tmp_user_ptr->nickName,
            args 
        );
        tmp_user_ptr->IRCPushMessage(resp);
        return NULL;
    }

    tmp_user_ptr->Login(tmp_user_ptr->socket);

    auto args = std::vector<std::string>();

    args.push_back(tmp_user_ptr->nickName);
    args.push_back(tmp_user_ptr->userName);
    args.push_back(tmp_user_ptr->ircHost);

    auto resp = irc::IRCResponse(
        server->GetSrc(),
        irc::RESP_CODE::RPL_WELCOME,
        tmp_user_ptr->nickName,
        args 
    );

    tmp_user_ptr->IRCPushMessage(resp);

    args.clear();
    args.push_back(tmp_user_ptr->ircHost);

    auto resp_1 = irc::IRCResponse(
        server->GetSrc(),
        irc::RESP_CODE::RPL_YOURHOST,
        tmp_user_ptr->nickName,
        args 
    );

    tmp_user_ptr->IRCPushMessage(resp_1);

    args.clear();

    auto resp_2 = irc::IRCResponse(
        server->GetSrc(),
        irc::RESP_CODE::RPL_CREATED,
        tmp_user_ptr->nickName,
        args 
    );

    tmp_user_ptr->IRCPushMessage(resp_2);

    args.clear();
    args.push_back(server->host);

    auto resp_3 = irc::IRCResponse(
        server->GetSrc(),
        irc::RESP_CODE::RPL_MYINFO,
        tmp_user_ptr->nickName,
        args 
    );

    tmp_user_ptr->IRCPushMessage(resp_3);

    Lusers(tmp_user_ptr, req);
    Motd(tmp_user_ptr, req);

    return tmp_user_ptr;
}

void irc::business::Chat(irc::User *user, irc::IRCRequest &req) {
    auto server = irc::Server::GetInstance();
    if(req.cmds.size() < 2) {
        auto args = std::vector<std::string>();
        args.push_back(req.cmds[0]);
        auto resp = irc::IRCResponse(
            server->GetSrc(),
            irc::RESP_CODE::ERR_NORECIPIENT,
            user->nickName,
            args 
        );
        user->IRCPushMessage(resp);
        return ;
    }
    if(req.cmds.size() == 2) {
        auto args = std::vector<std::string>();
        auto resp = irc::IRCResponse(
            server->GetSrc(),
            irc::RESP_CODE::ERR_NOTEXTTOSEND,
            user->nickName,
            args 
        );
        user->IRCPushMessage(resp);
        return ;
    }


    auto chat_nick = req.cmds[1];
    auto msg = req.cmds[2];

    if(chat_nick[0] == '#') {
        // 频道聊天

        auto chat_channel = server->ReadChannel(chat_nick);
        if (chat_channel == NULL) { // ERR_NOSUCHCHANNEL
            auto args = std::vector<std::string>();
            args.push_back(chat_nick);
            auto resp = irc::IRCResponse(
                server->GetSrc(),
                irc::RESP_CODE::ERR_NOSUCHNICK,
                user->nickName,
                args 
            );
            user->IRCPushMessage(resp);
            return ;
        } else {
            // 找到频道
            if(std::find(chat_channel->users.begin(), chat_channel->users.end(), user)==chat_channel->users.end()) {
                auto args = std::vector<std::string>();
                args.push_back(chat_nick);
                auto resp = irc::IRCResponse(
                    server->GetSrc(),
                    irc::RESP_CODE::ERR_CANNOTSENDTOCHAN,
                    user->nickName,
                    args 
                );
                user->IRCPushMessage(resp);
            } else {
                auto args = std::vector<std::string>();
                args.push_back("PRIVMSG");
                args.push_back(chat_channel->name);
                args.push_back(msg.substr(0, msg.size()-1));
                auto resp = irc::IRCResponse(
                        user->GetSrc(),
                        "",
                        user->nickName,
                        args
                );
                chat_channel->IRCBroadcast(resp);
            }
        }

    } else {
        // 用户聊天
        auto char_user = server->ReadUser(chat_nick);
        if (char_user == NULL) {
            // 处理没找到user
            auto args = std::vector<std::string>();
            args.push_back(chat_nick);
            auto resp = irc::IRCResponse(
                server->GetSrc(),
                irc::RESP_CODE::ERR_NOSUCHNICK,
                user->nickName,
                args 
            );
            user->IRCPushMessage(resp);
            return;
        } else {
            // 找到user
            auto args = std::vector<std::string>();
            args.push_back("PRIVMSG");
            args.push_back(chat_nick);
            args.push_back(msg.substr(0, msg.size()-1));
            auto resp = irc::IRCResponse(
                    user->GetSrc(),
                    "",
                    user->nickName,
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
    if(req.cmds.size() < 2) {
        auto args = std::vector<std::string>();
        args.push_back(req.cmds[0]);
        auto resp = irc::IRCResponse(
            server->GetSrc(),
            irc::RESP_CODE::ERR_NEEDMOREPARAMS,
            user->nickName,
            args 
        );
        user->IRCPushMessage(resp);
        return ;
    }

    auto char_channel_nick = req.cmds[1];

    auto char_channel = server->ReadChannel(char_channel_nick);
    if (char_channel == NULL) {
        char_channel = new irc::Channel();
        char_channel->name = char_channel_nick;
        server->SetChannel(char_channel_nick, char_channel);
    }
    if(std::find(char_channel->users.begin(), char_channel->users.end(), user)==char_channel->users.end())
        char_channel->users.push_back(user);
    else {
        return ;
    }

    std::vector<std::string> args;

    args.push_back(req.cmds[0]);
    args.push_back(req.cmds[1]);
    irc::IRCResponse resp_ack(
        user->GetSrc(),
        "",
        user->nickName,
        args
    );
    char_channel->IRCBroadcast(resp_ack);

    args.clear();
    args.push_back(char_channel->name);
    for(auto i:char_channel->users) {
        args.push_back(i->nickName);
    }
    irc::IRCResponse resp_names_list(
        user->GetSrc(),
        irc::RESP_CODE::RPL_NAMREPLY,
        user->nickName,
        args
    );
    user->IRCPushMessage(resp_names_list);

    args.clear();
    args.push_back(char_channel->name);
    irc::IRCResponse resp_end_list(
        user->GetSrc(),
        irc::RESP_CODE::RPL_ENDOFNAMES,
        user->nickName,
        args
    );
    user->IRCPushMessage(resp_end_list);
}

void irc::business::UnknowResp(irc::User *user, irc::IRCRequest &req){
    auto server = irc::Server::GetInstance();
    auto args = std::vector<std::string>();
    args.push_back(req.cmds[0]);
    auto resp = irc::IRCResponse(
            server->GetSrc(),
            irc::RESP_CODE::ERR_UNKNOWNCOMMAND,
            user->nickName,
            args
    );
    user->IRCPushMessage(resp);
}

void irc::business::UnKnowNickResp(irc::User *user, irc::IRCRequest &req){
    auto server = irc::Server::GetInstance();
    auto args = std::vector<std::string>();
    args.push_back(req.cmds[1]);
    auto resp = irc::IRCResponse(
          server->GetSrc(),
            irc::RESP_CODE::ERR_NOSUCHNICK,
            user->nickName,
            args
    );
    user->IRCPushMessage(resp);
};


void irc::business::Motd(irc::User *user , irc::IRCRequest &req) {
    auto server = irc::Server::GetInstance();
    std::vector<std::string> msgs;
    try {
        msgs = user->PushOfflineMessage();
    } catch (std::exception e) {
        auto args = std::vector<std::string>();
        auto resp = irc::IRCResponse(
            server->GetSrc(),
                irc::RESP_CODE::ERR_NOMOTD,
                user->nickName,
                args
        );
        user->IRCPushMessage(resp);
        LogC("NO motd file");
        return;
    }

    if(msgs.empty()) {
        auto args = std::vector<std::string>();
        auto resp = irc::IRCResponse(
            server->GetSrc(),
                irc::RESP_CODE::ERR_NOMOTD,
                user->nickName,
                args
        );
        user->IRCPushMessage(resp);
        LogC("no motd");
    } else {
        // RPL_MOTDSTART
        auto args = std::vector<std::string>();
        auto resp = irc::IRCResponse(
            server->GetSrc(),
                irc::RESP_CODE::RPL_MOTDSTART,
                user->nickName,
                args
        );
        user->IRCPushMessage(resp);
        LogC("start motd");

        for(auto msg:msgs) {
            auto args = std::vector<std::string>();
            args.push_back(msg);
            auto resp = irc::IRCResponse(
                server->GetSrc(),
                    irc::RESP_CODE::RPL_MOTD,
                    user->nickName,
                    args
            );
            user->IRCPushMessage(resp);
            LogC("motd");
        }

        args.clear();
        // RPL_ENDOFMOTD
        resp = irc::IRCResponse(
            server->GetSrc(),
            irc::RESP_CODE::RPL_ENDOFMOTD,
            user->nickName,
            args
        );
        user->IRCPushMessage(resp);
        LogC("end motd");
    }
}

void irc::business::PartChannel(irc::User *user, irc::IRCRequest &req)
{
    auto server = irc::Server::GetInstance();
    if(req.cmds.size() < 2) {
        auto args = std::vector<std::string>();
        args.push_back(req.cmds[0]);
        auto resp = irc::IRCResponse(
            server->GetSrc(),
            irc::RESP_CODE::ERR_NEEDMOREPARAMS,
            user->nickName,
            args 
        );
        user->IRCPushMessage(resp);
        return ;
    }

    auto char_channel_nick = req.cmds[1];

    auto char_channel = server->ReadChannel(char_channel_nick);
    if (char_channel == NULL) {
        auto args = std::vector<std::string>();
        args.push_back(char_channel_nick);
        auto resp = irc::IRCResponse(
            server->GetSrc(),
            irc::RESP_CODE::ERR_NOSUCHCHANNEL,
            user->nickName,
            args 
        );
        user->IRCPushMessage(resp);
        return ;
    }
    if(std::find(char_channel->users.begin(), char_channel->users.end(), user)==char_channel->users.end())
        return;
    std::vector<std::string> args(req.cmds);
    irc::IRCResponse resp_ack(
        user->GetSrc(),
        "",
        user->nickName,
        args
    );
    char_channel->IRCBroadcast(resp_ack);
    char_channel->users.remove(user);
}

void irc::business::Quit(irc::User *user, irc::IRCRequest &req)
{
    auto server = irc::Server::GetInstance();
    auto args = std::vector<std::string>();
    args.push_back("ERROR :Closing Link:");
    args.push_back(server->GetSrc());
    if(req.cmds.size() > 1) {
        args.push_back("("+ req.cmds[1].substr(1, req.cmds[1].size()-2) +")");
    } else {
        args.push_back("(Client Quit)");
    }
    
    auto resp = irc::IRCResponse(
        server->GetSrc(),
        "",
        user->nickName,
        args
    );
    user->IRCPushMessage(resp);
    user->Logout();
}

void irc::business::Lusers(irc::User *user, irc::IRCRequest &req)
{
    // RPL_LUSERCLIENT
    auto server = irc::Server::GetInstance();
    auto args = std::vector<std::string>();
    args.push_back(std::to_string(server->UserMap.size()));
    auto resp1 = irc::IRCResponse(
        server->GetSrc(),
        irc::RESP_CODE::RPL_LUSERCLIENT,
        user->nickName,
        args
    );
    user->IRCPushMessage(resp1);
    // RPL_LUSEROP
    args.clear();
    args.push_back(std::to_string(0));
    auto resp2 = irc::IRCResponse(
        server->GetSrc(),
        irc::RESP_CODE::RPL_LUSEROP,
        user->nickName,
        args
    );
    user->IRCPushMessage(resp2);
    // RPL_LUSERUNKNOWN
    args.clear();
    args.push_back(std::to_string(server->unknowns));
    auto resp3 = irc::IRCResponse(
        server->GetSrc(),
        irc::RESP_CODE::RPL_LUSERUNKNOWN,
        user->nickName,
        args
    );
    user->IRCPushMessage(resp3);
    // RPL_LUSERCHANNELS
    args.clear();
    args.push_back(std::to_string(server->ChannelMap.size()));
    auto resp4 = irc::IRCResponse(
        server->GetSrc(),
        irc::RESP_CODE::RPL_LUSERCHANNELS,
        user->nickName,
        args
    );
    user->IRCPushMessage(resp4);
    // RPL_LUSERME
    args.clear();
    int n = 0;
    for(auto client:server->UserMap) {
        if(client.second->state)
            n++;
    }
    args.push_back(std::to_string(n + server->unknowns));
    auto resp5 = irc::IRCResponse(
        server->GetSrc(),
        irc::RESP_CODE::RPL_LUSERME,
        user->nickName,
        args
    );
    user->IRCPushMessage(resp5);
}

void irc::business::NAMES(irc::User *user, irc::IRCRequest &req) {
    auto server = irc::Server::GetInstance();
    if (req.cmds.size() == 1){
        for(auto channel : server->ChannelMap) {
            std::vector<std::string> args;
            args.push_back(channel.first);
            for(auto _user : channel.second->users) {
                args.push_back(_user->nickName);
            }
            irc::IRCResponse msg(
                server->GetSrc(),
                irc::RESP_CODE::RPL_NAMREPLY,
                user->nickName,
                args
            );
            user->IRCPushMessage(msg);
        }
    } else if (req.cmds.size() == 2) {

        auto char_channel_nick = req.cmds[1];

        auto char_channel = server->ReadChannel(char_channel_nick);
        if (char_channel == NULL) {
            UnKnowNickResp(user, req);
            return;
        }
        std::vector<std::string> args;
        args.push_back(char_channel->name);
        for(auto _user : char_channel->users) {
            args.push_back(_user->nickName);
        }
        irc::IRCResponse msg(
            server->GetSrc(),
            irc::RESP_CODE::RPL_NAMREPLY,
            user->nickName,
            args
        );
        user->IRCPushMessage(msg);

    } else {
        UnknowResp(user, req);
    }
    
}

void irc::business::Notice(irc::User *user, irc::IRCRequest &req)
{
    auto server = irc::Server::GetInstance();
    if(req.cmds.size() < 2) {
        return ;
    }
    if(req.cmds.size() == 2) {
        return ;
    }


    auto chat_nick = req.cmds[1];
    auto msg = req.cmds[2];

    if(chat_nick[0] == '#') {
        // 频道聊天

        auto chat_channel = server->ReadChannel(chat_nick);
        if (chat_channel == NULL) { // ERR_NOSUCHCHANNEL
            return ;
        } else {
            // 找到频道
            if(std::find(chat_channel->users.begin(), chat_channel->users.end(), user)==chat_channel->users.end()) {
                return ;
            } else {
                auto args = std::vector<std::string>();
                args.push_back("NOTICE");
                args.push_back(user->userName);
                args.push_back(msg.substr(0, msg.size()-1));
                auto resp = irc::IRCResponse(
                        server->GetSrc(),
                        "",
                        user->nickName,
                        args
                );
                chat_channel->IRCBroadcast(resp);
            }
        }

    } else {
        // 用户聊天
        auto char_user = server->ReadUser(chat_nick);
        if (char_user == NULL) {
            // 处理没找到user
            return;
        } else {
            // 找到user
            auto args = std::vector<std::string>();
            args.push_back("NOTICE");
            args.push_back(chat_nick);
            args.push_back(msg.substr(0, msg.size()-1));
            auto resp = irc::IRCResponse(
                    user->GetSrc(),
                    "",
                    user->nickName,
                    args
            );
            char_user->IRCPushMessage(resp);
            return;
        }
    }
}

void irc::business::Ping(irc::User *user, irc::IRCRequest &req)
{
    auto server = irc::Server::GetInstance();
    auto args = std::vector<std::string>();
    args.push_back("PONG");
    args.push_back(user->userName);
    
    auto resp = irc::IRCResponse(
        server->GetSrc(),
        "",
        user->nickName,
        args
    );
    user->IRCPushMessage(resp);
}

void irc::business::Whois(irc::User *user, irc::IRCRequest &req)
{

}
