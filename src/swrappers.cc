#include "swrappers.hh"
#include <map>
#include <unistd.h>
#include "util.h"
#include <fcntl.h>

/** these functions provide a very lightweight wrapper to the Berkeley sockets API. Errors -> exceptions! */

static void RuntimeError(const std::string& fmt)
{
  throw std::runtime_error(fmt);
}


int SSocket(int family, int type, int flags)
{
  int ret = socket(family, type, flags);
  if(ret < 0)
    RuntimeError(strerror(errno));
  return ret;
}

int SConnect(int sockfd, const ComboAddress& remote)
{
  int ret = connect(sockfd, (struct sockaddr*)&remote, remote.getSocklen());
  if(ret < 0) {
    int savederrno = errno;
    RuntimeError(strerror(errno));
  }
  return ret;
}

int SBind(int sockfd, const ComboAddress& local)
{
  int ret = bind(sockfd, (struct sockaddr*)&local, local.getSocklen());
  if(ret < 0) {
    int savederrno = errno;
    RuntimeError(strerror(errno));
  }
  return ret;
}

int SAccept(int sockfd, ComboAddress& remote)
{
  socklen_t remlen = remote.getSocklen();

  int ret = accept(sockfd, (struct sockaddr*)&remote, &remlen);
  if(ret < 0)
    RuntimeError(strerror(errno));
  return ret;
}

int SListen(int sockfd, int limit)
{
  int ret = listen(sockfd, limit);
  if(ret < 0)
    RuntimeError(strerror(errno));
  return ret;
}

int SSetsockopt(int sockfd, int level, int opname, int value)
{
  int ret = setsockopt(sockfd, level, opname, &value, sizeof(value));
  if(ret < 0)
    RuntimeError(strerror(errno));
  return ret;
}

void SWrite(int sockfd, const std::string& content, std::string::size_type *wrlen)
{
  int res = write(sockfd, &content[0], content.size());
  if(res < 0)
    RuntimeError(strerror(errno));
  if(wrlen) 
    *wrlen = res;

  if(res != (int)content.size()) {
    if(wrlen) {
      return;
    }
    RuntimeError(strerror(errno));
  }
}

void SWriten(int sockfd, const std::string& content)
{
  std::string::size_type pos=0;
  for(;;) {
    int res = write(sockfd, &content[pos], content.size()-pos);
    if(res < 0)
      RuntimeError(strerror(errno));
    if(!res)
      RuntimeError(strerror(errno));
    pos += res;
    if(pos == content.size())
      break;
  }
}

std::string SRead(int sockfd, std::string::size_type limit)
{
  std::string ret;
  char buffer[1024];
  std::string::size_type leftToRead=limit;
  for(; leftToRead;) {
    auto chunk = sizeof(buffer) < leftToRead ? sizeof(buffer) : leftToRead;
    int res = read(sockfd, buffer, chunk);
    if(res < 0)
      RuntimeError(strerror(errno));
    if(!res)
      RuntimeError(strerror(errno));
    ret.append(buffer, res);
    leftToRead -= res;
    if(IsEndWith(ret, "\r\n")){
      break;
    }
  }
  return ret;
}

void SSendto(int sockfd, const std::string& content, const ComboAddress& dest, int flags)
{
  int ret = sendto(sockfd, &content[0], content.size(), flags, (struct sockaddr*)&dest, dest.getSocklen());
  if(ret < 0)
    RuntimeError(strerror(errno));
}

int SSend(int sockfd, const std::string& content, int flags)
{
  int ret = send(sockfd, &content[0], content.size(), flags);
  if(ret < 0)
    RuntimeError(strerror(errno));
  return ret;
}


std::string SRecvfrom(int sockfd, std::string::size_type limit, ComboAddress& dest, int flags)
{
  std::string ret;
  ret.resize(limit);
  
  socklen_t slen = dest.getSocklen();
  int res = recvfrom(sockfd, &ret[0], ret.size(), flags, (struct sockaddr*)&dest, &slen);
  
  if(res < 0)
    RuntimeError(strerror(errno));
    
  ret.resize(res);
  return ret;
}

void SGetsockname(int sock, ComboAddress& orig)
{
  socklen_t slen=orig.getSocklen();
  if(getsockname(sock, (struct sockaddr*)&orig, &slen) < 0)
    RuntimeError(strerror(errno));
}


void SetNonBlocking(int sock, bool to)
{
  int flags=fcntl(sock,F_GETFL,0);
  if(flags<0)
    RuntimeError(strerror(errno));

  // so we could optimize to not do it if nonblocking already set, but that would be.. semantics
  if(to) {
    flags |= O_NONBLOCK;
  }
  else 
    flags &= (~O_NONBLOCK);
      
  if(fcntl(sock, F_SETFL, flags) < 0)
    RuntimeError(strerror(errno));
}

std::map<int, short> SPoll(const std::vector<int>&rdfds, const std::vector<int>&wrfds, double timeout)
{
  std::vector<pollfd> pfds;
  std::map<int, short> inputs;
  for(const auto& i : rdfds) {
    inputs[i]|=POLLIN;
  }
  for(const auto& i : wrfds) {
    inputs[i]|=POLLOUT;
  }
  for(const auto& p : inputs) {
    pfds.push_back({p.first, p.second, 0});
  }
  int res = poll(&pfds[0], pfds.size(), timeout*1000);
  if(res < 0)
    RuntimeError(strerror(errno));
  inputs.clear();
  if(res) {
    for(const auto& pfd : pfds) {
      if((pfd.revents & pfd.events) || pfd.revents==POLLERR )
        inputs[pfd.fd]=pfd.revents;
    }
  }
  return inputs;
}

std::vector<ComboAddress> resolveName(const std::string& name, bool ipv4, bool ipv6)
{
  std::vector<ComboAddress> ret;

  for(int n = 0; n < 2; ++n) {
    struct addrinfo* res;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = n ? AF_INET : AF_INET6;
    if(hints.ai_family == AF_INET && !ipv4)
      continue;
    if(hints.ai_family == AF_INET6 && !ipv6)
      continue;
    
    ComboAddress remote;
    remote.sin4.sin_family = AF_INET6;
    if(!getaddrinfo(&name[0], 0, &hints, &res)) { // this is how the getaddrinfo return code works
      struct addrinfo* address = res;
      do {
        if (address->ai_addrlen <= sizeof(remote)) {
          memcpy(&remote, address->ai_addr, address->ai_addrlen);
          remote.sin4.sin_port=0;
          ret.push_back(remote);
        }
      } while((address = address->ai_next));
      freeaddrinfo(res);
    }
  }
  return ret;
}

