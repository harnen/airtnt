//
// blocking_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>
#include <boost/asio.hpp>

#include "chat_message.hpp"
using boost::asio::ip::tcp;

const int max_length = 1024;
chat_message read_msg_;

void session(tcp::socket sock)
{
  try
  {
    for (;;)
    {
      char data[max_length];

      boost::system::error_code error;
      size_t length = sock.read_some(boost::asio::buffer(read_msg_.data(), chat_message::header_length), error);
      if (error == boost::asio::error::eof){
        std::cout << "Connection close by peer\n";
        break; // Connection closed cleanly by peer.
      }else if (error){
        throw boost::system::system_error(error); // Some other error.
      }

      if( !read_msg_.decode_header()){
        std::cout << "Error reading header\n";
        break;
      }
      sock.read_some(boost::asio::buffer(read_msg_.body(), read_msg_.body_length()), error);
      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      boost::asio::write(sock, boost::asio::buffer(read_msg_.body(), read_msg_.body_length()));
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

void server(boost::asio::io_context& io_context, unsigned short port)
{
  tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
  for (;;)
  {
    std::thread(session, a.accept()).detach();
  }
}

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: blocking_tcp_echo_server <port>\n";
      return 1;
    }

    boost::asio::io_context io_context;

    server(io_context, std::atoi(argv[1]));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
