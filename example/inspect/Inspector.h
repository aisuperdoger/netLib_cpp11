// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef MUDUO_NET_INSPECT_INSPECTOR_H
#define MUDUO_NET_INSPECT_INSPECTOR_H

#include "muduo/base/Mutex.h"
#include "muduo/net/http/HttpRequest.h"
#include "muduo/net/http/HttpServer.h"

#include <map>

namespace muduo
{
namespace net
{

class ProcessInspector;
class PerformanceInspector;
class SystemInspector;

// An internal inspector of the running process, usually a singleton.
// Better to run in a seperated thread, as some method may block for seconds
class Inspector : noncopyable
{
 public:
  typedef std::vector<string> ArgList;
  typedef std::function<string (HttpRequest::Method, const ArgList& args)> Callback;
  Inspector(EventLoop* loop,
            const InetAddress& httpAddr,
            const string& name);
  ~Inspector();

  /// Add a Callback for handling the special uri : /mudule/command
  // 假设url为/proc/opened_files，则module为proc，command为opened_files
  void add(const string& module,
           const string& command,
           const Callback& cb,
           const string& help);
  void remove(const string& module, const string& command);

 private:
  typedef std::map<string, Callback> CommandList; // 假设url为/proc/opened_files   那么CommandList为<opened_files, callback>
  typedef std::map<string, string> HelpList;      // opened_files和opened_files的帮助信息

  void start();
  void onRequest(const HttpRequest& req, HttpResponse* resp);

  HttpServer server_;
  std::unique_ptr<ProcessInspector> processInspector_; // processInspector_指的是proc开头的url  定义类似processInspector_的类就可以定义出不同的url。
  std::unique_ptr<PerformanceInspector> performanceInspector_;
  std::unique_ptr<SystemInspector> systemInspector_;
  MutexLock mutex_;
  std::map<string, CommandList> modules_ GUARDED_BY(mutex_);  // 假设url为/proc/opened_files   那么modules_为<proc, CommandList>
  std::map<string, HelpList> helps_ GUARDED_BY(mutex_);
};

}  // namespace net
}  // namespace muduo

#endif  // MUDUO_NET_INSPECT_INSPECTOR_H
