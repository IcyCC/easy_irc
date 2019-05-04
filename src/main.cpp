#include"irc_user.h"
#include"irc_server.h"
#include"irc_request.h"
#include "irc_response.h"
#include<iostream>

std::mutex g_lock;


int main (){
    auto server = irc::Server::Server::GetInstance();
    server->RunServe();
    return 0;
}