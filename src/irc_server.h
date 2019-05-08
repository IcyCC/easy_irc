#pragma once
#include <map>
#include "irc_error.h"
#include "irc_response.h"
#include "irc_request.h"
#include "irc_channel.h"
#include "irc_user.h"
#include "irc_business.h"
#include "util.h"
#include <thread>
#include <mutex>

extern std::mutex g_lock;

namespace irc
{
class Server
{
  public:
    static Server *_server;

  public:
    std::string host;
    std::string port;
    int unknowns;

  public:
    std::map<std::string, irc::Channel *> ChannelMap;
    std::map<std::string, irc::User *> UserMap;

  public:
    irc::User *ReadUser(std::string name);
    irc::Channel *ReadChannel(std::string name);
    void SetUser(std::string name, irc::User *user);
    void SetChannel(std::string name, irc::Channel *Channel);
  
  public:
    void RunServe(); // 运行服务
    static Server *GetInstance();
    Server();
    inline std::string GetSrc() {
      return host + "#" + port;
    }
    void ReduceUnknown();
};
} // namespace irc
