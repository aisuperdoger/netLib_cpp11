#pragma once

#include <functional>
#include <thread>
#include <memory>
#include <unistd.h>
#include <string>
#include <atomic>

#include "noncopyable.h"

class Thread : noncopyable
{
public:
    using ThreadFunc = std::function<void()>; // 线程对应的回调函数

    explicit Thread(ThreadFunc, const std::string &name = std::string()); // 输入：线程函数，线程名
    ~Thread();

    void start();
    void join();

    bool started() { return started_; } // 线程是否启动
    pid_t tid() const { return tid_; }
    const std::string &name() const { return name_; }

    static int numCreated() { return numCreated_; }

private:
    void setDefaultName();

    bool started_;
    bool joined_;
    std::shared_ptr<std::thread> thread_;
    pid_t tid_;       // 在线程创建时再绑定
    ThreadFunc func_; // 线程回调函数
    std::string name_;
    static std::atomic_int numCreated_;
};