// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_HTTP_HTTPCONTEXT_H
#define MUDUO_NET_HTTP_HTTPCONTEXT_H

#include "muduo/base/copyable.h"

#include "muduo/net/http/HttpRequest.h"

namespace muduo
{
namespace net
{

class Buffer;

class HttpContext : public muduo::copyable
{
 public:
  enum HttpRequestParseState
  {
    kExpectRequestLine, // 正处于解析请求行的状态
    kExpectHeaders, // 正处于解析请求头的状态
    kExpectBody,  // 正处于解析请求体的状态，当前未实现解析body的功能
    kGotAll,  // 请求解析完毕
  };

  HttpContext() 
    : state_(kExpectRequestLine) 
  {
  }

  // default copy-ctor, dtor and assignment are fine

  // return false if any error
  bool parseRequest(Buffer* buf, Timestamp receiveTime);

  bool gotAll() const
  { return state_ == kGotAll; }

  void reset()
  {
    state_ = kExpectRequestLine;
    HttpRequest dummy; // 用于交换，交换后，request_就是一个空的HttpRequest对象
    request_.swap(dummy); // 交换两个对象的内容
  }

  const HttpRequest& request() const
  { return request_; }

  HttpRequest& request()
  { return request_; }

 private:
  bool processRequestLine(const char* begin, const char* end);

  HttpRequestParseState state_;
  HttpRequest request_;
};

}  // namespace net
}  // namespace muduo

#endif  // MUDUO_NET_HTTP_HTTPCONTEXT_H
