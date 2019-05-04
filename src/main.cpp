#include"irc_user.h"
#include"irc_server.h"
#include"irc_request.h"
#include "irc_response.h"
#include<iostream>

std::mutex g_lock;

void test_Request (){
    std::string t = "NICK username\r\n";
    auto r = irc::IRCRequest(t);
    std::cout<<r.cmds[0]<<std::endl;
};

int main (){
    test_Request();
    std::cout<<irc::RESP_CODE::ERR_NICKNAMEINUSE;
    return 0;
}