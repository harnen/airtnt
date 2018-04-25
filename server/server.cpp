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


////////////////////////////////
// EDIT
////////////////////////////////
#include "network_ra.h"

#ifndef SAFE_FREE
#define SAFE_FREE(ptr) {if (NULL != (ptr)) {free(ptr); (ptr) = NULL;}}
#endif
////////////////////////////////
// END EDIT
////////////////////////////////

void session(tcp::socket sock)
{
  try
  {
    for (;;)
    {
      char data[max_length];

      boost::system::error_code error;
      size_t length = sock.read_some(boost::asio::buffer(data, sizeof(ra_samp_request_header_t)), error);
      if (error == boost::asio::error::eof){
        std::cout << "Connection close by peer\n";
        break; // Connection closed cleanly by peer.
      }else if (error){
        throw boost::system::system_error(error); // Some other error.
      }

      /*
      if( !read_msg_.decode_header()){
        std::cout << "Error reading header\n";
        break;
      }
      */
      ra_samp_request_header_t *header = (ra_samp_request_header_t*) data;
      printf("%d\n", header->size);
      printf("%d\n", header->type);

      sock.read_some(boost::asio::buffer(data+sizeof(*header), header->size), error);
      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.



      ////////////////////////////////
      // EDIT
      ////////////////////////////////
      // set message
      /*
      ra_samp_request_header_t *p_msg0_full = NULL;
      p_msg0_full = (ra_samp_request_header_t*)
        malloc(sizeof(ra_samp_request_header_t) + sizeof(uint32_t));
      p_msg0_full->type = read_msg_.type(); // e.g., TYPE_RA_MSG0;
      p_msg0_full->size = sizeof(uint32_t);
      uint32_t extended_epid_group_id = 0;
      *(uint32_t*)((uint8_t*)p_msg0_full + sizeof(ra_samp_request_header_t)) = extended_epid_group_id;

      // et buffer
      ra_samp_response_header_t *p_msg0_resp_full = NULL;
      */

      ra_samp_response_header_t *p_msg0_resp_full = NULL;

      // receive message
      ra_network_send_receive(
        "http://example.com",
        (ra_samp_request_header_t*) data,
        &p_msg0_resp_full
      );

      printf("Size: %d\n", p_msg0_resp_full->size);


      // free mem
      ra_free_network_response_buffer(p_msg0_resp_full);
      //SAFE_FREE(p_msg0_full);
      ////////////////////////////////
      // END EDIT
      ////////////////////////////////



      // wirte to dump to socket
      boost::asio::write(sock, boost::asio::buffer(
        p_msg0_resp_full, 
        sizeof(ra_samp_response_header_t) + p_msg0_resp_full->size)
      );



      ////////////////////////////////
      // EDIT
      ////////////////////////////////
      // free mem
      ra_free_network_response_buffer(p_msg0_resp_full);
      ////////////////////////////////
      // END EDIT
      ////////////////////////////////
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
