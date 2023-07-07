// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef MUDUO_NET_HTTP_HTTPRESPONSE_H
#define MUDUO_NET_HTTP_HTTPRESPONSE_H

#include "muduo/base/copyable.h"
#include "muduo/base/Types.h"

#include <map>

namespace muduo
{
namespace net
{

class Buffer;
class HttpResponse : public muduo::copyable
{
 public:
  enum HttpStatusCode
  {
    kUnknown,
    k200Ok = 200, 
    k301MovedPermanently = 301, // 301重定向.请求的页面永久性移至另-个地址
    k400BadRequest = 400,  // 请求错误
    k404NotFound = 404,  // 请求的页面不存在
  };

  explicit HttpResponse(bool close)
    : statusCode_(kUnknown),
      closeConnection_(close)
  {
  }

  void setStatusCode(HttpStatusCode code)
  { statusCode_ = code; }

  void setStatusMessage(const string& message)
  { statusMessage_ = message; }

  void setCloseConnection(bool on)
  { closeConnection_ = on; }

  bool closeConnection() const
  { return closeConnection_; }

  // 设置文档类型(MIME)，如text/html
  void setContentType(const string& contentType)
  { addHeader("Content-Type", contentType); }

  // FIXME: replace string with StringPiece
  void addHeader(const string& key, const string& value)
  { headers_[key] = value; }

  void setBody(const string& body)
  { body_ = body; }

  // 将HttpResponse对象中的内容转换为字符串，发送给客户端
  void appendToBuffer(Buffer* output) const; 

 private:
  std::map<string, string> headers_;
  HttpStatusCode statusCode_;
  // FIXME: add http version
  string statusMessage_; // statusCode_对应的描述信息
  bool closeConnection_; // 是否关闭连接 即是否是短连接
  string body_;
};

}  // namespace net
}  // namespace muduo

#endif  // MUDUO_NET_HTTP_HTTPRESPONSE_H
