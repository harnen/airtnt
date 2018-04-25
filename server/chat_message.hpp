//
// chat_message.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>

class chat_message
{
public:
  enum { header_length = 2 };
  enum { max_body_length = 255 };

  chat_message()
    : body_length_(0)
  {
  }

  const char* data() const
  {
    return data_;
  }

  char* data()
  {
    return data_;
  }

  std::size_t length() const
  {
    return header_length + body_length_;
  }

  const char* body() const
  {
    return data_ + header_length;
  }

  char* body()
  {
    return data_ + header_length;
  }

  std::size_t body_length() const
  {
    return body_length_;
  }

  uint8_t type() const
  {
    return msg_type_;
  }

  void type(uint8_t new_type)
  {
    msg_type_ = new_type;
  }

  void body_length(std::size_t new_length)
  {
    body_length_ = new_length;
    if (body_length_ > max_body_length)
      body_length_ = max_body_length;
  }

  bool decode_header()
  {
    body_length_ = (uint8_t) data_[0];
    if (body_length_ > max_body_length)
    {
      body_length_ = 0;
      return false;
    }

    msg_type_ = data_[1];
    return true;
  }

  void encode_header()
  {
    data_[0] = body_length_;
    data_[1] = msg_type_;
  }

private:
  char data_[header_length + max_body_length];
  uint8_t body_length_;
  uint8_t msg_type_;
};

#endif // CHAT_MESSAGE_HPP
