#pragma once

#include <functional>

#include "noncopyable.h"
#include "Socket.h"
#include "Channel.h"

class EventLoop;
class InetAddress;

class Acceptor : noncopyable
{
public:
    using NewConnectionCallback = std::function<void(int sockfd, const InetAddress &)>;

    Acceptor(EventLoop *loop, const InetAddress &listenAddr, bool reuseport);
    ~Acceptor();

    void setNewConnectionCallback(const NewConnectionCallback &cb) { NewConnectionCallback_ = cb; } 

    bool listenning() const { return listenning_; }
    void listen();

private:
    void handleRead();  // handleRead()被设置为acceptChannel_的读事件的回调函数

    EventLoop *loop_; // Acceptor用的就是用户定义的那个baseLoop 也称作mainLoop
    Socket acceptSocket_;
    Channel acceptChannel_;
    NewConnectionCallback NewConnectionCallback_; // TcpServer.cc中有设置这个回调函数
    bool listenning_;
};