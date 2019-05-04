#include "irc_server.h"

irc::Server* irc::Server::_server = NULL;
irc::Server::Server()
{
    host = "127.0.0.1";
    port = "10086";
}

irc::Server *irc::Server::GetInstance()
{
    g_lock.lock();
    if (_server == NULL)
    {
        g_lock.unlock();
        _server = new irc::Server();
    }
    g_lock.unlock();
    return _server;
}

irc::User *irc::Server::ReadUser(std::string name)
{
    g_lock.lock();
    auto user = UserMap.find(name);
    if (user == UserMap.end())
    {
        g_lock.unlock();
        return NULL;
    }
    g_lock.unlock();
    return user->second;
}

irc::Channel *irc::Server::ReadChannel(std::string name)
{
    g_lock.lock();
    auto channel = ChannelMap.find(name);
    if (channel == ChannelMap.find(name))
    {
        g_lock.unlock();
        return NULL;
    }
    g_lock.unlock();
    return channel->second;
}

void irc::Server::SetUser(std::string name, irc::User *user)
{
    g_lock.lock();
    UserMap.insert(std::pair<std::string, irc::User *>(name, user));
    g_lock.unlock();
}

void irc::Server::SetChannel(std::string name, irc::Channel *channel)
{
    g_lock.lock();
    ChannelMap.insert(std::pair<std::string, irc::Channel *>(name, channel));
    g_lock.unlock();
}

void irc::Server::RunServe()
{
    int server_fd = SSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    ComboAddress local_addr(host+":"+port);
    SBind(server_fd, local_addr);
    SListen(server_fd, SOMAXCONN);
    int client_fd;
    ComboAddress client_addr = ComboAddress();
    while (client_fd = SAccept(server_fd, client_addr))
    {
        auto user = new irc::User();
        user->socket = client_fd;
        LogC("收到链接请求");
        std::thread t(irc::business::MainLogic, user);
        t.detach();
    }
}