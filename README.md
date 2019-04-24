# simple_irc

## 约定

## 实体

### 用户(存放套接字) User

属性:

```
username
name
_socket
unread
```

方法

```
IRCPushMessage(IRCMessage) int 给该用户发送消息
IRCRead() IRCRequest 从用户套接字读取数据 
```

### 频道  Channel

属性:

```
users
name
```

方法

```
IRCBroadcast(IRCMessage)
IRCJoin(User)
```

### 客户端请求 IRCRequest

属性:

```
raw
op
args
```


### 服务器响应 IRCResponse

属性
```
hostname
code
msg
```

方法

```
ToString
```

### 服务器 IRCServer

属性

```
users_map # 全局用户表
channel_map # 全局频道表
_socket # listen fd
```

方法

```
RunServe(int port)
```


### 整体流程

执行 IRCServer.Runserve 后, 初始化服务器的套接字, 并且监听链接, 

当接受到用户 建立链接后, 开一个线程, 处理用户的登录请求,  (此后所有步骤都处理成IRCRequest)

用户的登录请求会建立一个新的用户, 并且放入 IRCServer的表中 , 对 IRCServe表的操作全部加锁, 
并且在该线程内 while( req = user.IRCRead())


####  聊天

收到 req 根据 req 的参数 从 IRCServer 找到发送的User 调用 PushMessage  发送失败时候把消息
放入unread, 用户登录时候推给用户

### 频道

处理方法类似


