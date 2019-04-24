#include"irc_user.h"
#include"irc_server.h"

int main (){
    auto server = irc::Server::GetInstance();
    server->RunServe();
}