#include"irc_user.h"
#include"irc_server.h"
#include"irc_request.h"
#include<iostream>

void test_Request (){
    std::string t = "NICK username\r\n";
    auto r = irc::IRCRequest(t);
    std::cout<<r.cmds[0]<<std::endl;
};

int main (){
    test_Request();
    return 0;
}