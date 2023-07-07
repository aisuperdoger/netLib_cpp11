// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//

#include "muduo/net/Buffer.h"
#include "muduo/net/http/HttpContext.h"

using namespace muduo;
using namespace muduo::net;

bool HttpContext::processRequestLine(const char* begin, const char* end)
{
  bool succeed = false;
  const char* start = begin;
  const char* space = std::find(start, end, ' '); // 找到空格的位置
  if (space != end && request_.setMethod(start, space)) // 解析请求方法，如果GET
  {
    start = space+1;
    space = std::find(start, end, ' '); // 找到下一个空格的位置
    if (space != end) 
    {
      const char* question = std::find(start, space, '?');  // 解析路径 找到路径中问号的位置
      if (question != space)
      {
        request_.setPath(start, question);
        request_.setQuery(question, space);
      }
      else
      {
        request_.setPath(start, space);
      }
      start = space+1;
      succeed = end-start == 8 && std::equal(start, end-1, "HTTP/1.");  
      if (succeed)
      {
        if (*(end-1) == '1')
        {
          request_.setVersion(HttpRequest::kHttp11);  // HTTP/1.1 
        }
        else if (*(end-1) == '0')
        {
          request_.setVersion(HttpRequest::kHttp10);  // HTTP/1.0
        }
        else
        {
          succeed = false;
        }
      }
    }
  }
  return succeed;
}

// return false if any error
bool HttpContext::parseRequest(Buffer* buf, Timestamp receiveTime)
{
  bool ok = true;
  bool hasMore = true;
  while (hasMore) // 状态机
  {
    if (state_ == kExpectRequestLine)
    {
      const char* crlf = buf->findCRLF(); // 找到第一个\r\n，返回指向\r的指针
      if (crlf)
      {
        ok = processRequestLine(buf->peek(), crlf);
        if (ok)
        {
          request_.setReceiveTime(receiveTime); // 设置请求的接收时间
          buf->retrieveUntil(crlf + 2);         // 从缓冲区中取出请求行
          state_ = kExpectHeaders;
        }
        else
        {
          hasMore = false;
        }
      }
      else
      {
        hasMore = false;
      }
    }
    else if (state_ == kExpectHeaders)
    {
      const char* crlf = buf->findCRLF();
      if (crlf)
      {
        const char* colon = std::find(buf->peek(), crlf, ':'); // 找到第一个冒号的位置
        if (colon != crlf)
        {
          request_.addHeader(buf->peek(), colon, crlf); // 解析请求头，将请求头添加到headers_中
        }
        else
        {
          // empty line, end of header
          // FIXME:
          state_ = kGotAll; // 请求头解析完毕
          hasMore = false;
        }
        buf->retrieveUntil(crlf + 2);
      }
      else
      {
        hasMore = false;
      }
    }
    else if (state_ == kExpectBody) // 当前未实现解析body的功能
    {
      // FIXME:
    }
  }
  return ok;
}
