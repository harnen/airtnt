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

#include "misc.h"

#include "chat_message.hpp"
using boost::asio::ip::tcp;

const int max_length = 20000;
chat_message read_msg_;


////////////////////////////////
// EDIT
////////////////////////////////
#include "network_ra.h"
#include <time.h>

#ifndef SAFE_FREE
#define SAFE_FREE(ptr) {if (NULL != (ptr)) {free(ptr); (ptr) = NULL;}}
#endif

int steps=0;
int max_iterations=0;

int iter_counter=0;
////////////////////////////////
// END EDIT
////////////////////////////////

void session(tcp::socket sock)
{
    struct timeval t_connected, t_finished;
    gettimeofday(&t_connected,NULL);
    unsigned long m_connected = 1000000 * t_connected.tv_sec + t_connected.tv_usec;
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
      iter_counter++;

      ra_samp_request_header_t *header = (ra_samp_request_header_t*) data;

      

      #ifdef MYDEBUG 
      printf("[header] length: %d\n", length);
      printf("[header] received size: %d\n", header->size);
      printf("[header] received type: %d\n", header->type);
      #endif
        
      int sum=0;
      int read_bytes=0;
      
      while(sum < header->size) {

        read_bytes += sock.read_some(boost::asio::buffer(
          data+sizeof(ra_samp_request_header_t)+sum, header->size - sum), error
        );
        sum += read_bytes;

        if (error == boost::asio::error::eof){
          break; // Connection closed cleanly by peer.
        }else if (error){
          throw boost::system::system_error(error); // Some other error.
        }
      }
      
      
      #ifdef MYDEBUG 
      ra_samp_request_header_t* tmp = (ra_samp_request_header_t*) data;
      printf("[body] length: %d\n", read_bytes);
      printf("[body] received type: %d\n", tmp->type);
      printf("[body] received size: %d\n", tmp->size);
      printf("\n");
      #endif

      // receive message
      ra_samp_response_header_t *p_msg0_resp_full = NULL;

      ra_network_send_receive(
        "http://example.com",
        (ra_samp_request_header_t*) data,
        &p_msg0_resp_full,
        steps, 
        max_iterations
      );

      #ifdef MYDEBUG 
      printf("p_att_result_msg pointer: %d\n", p_msg0_resp_full);

      printf("Response size: %d\n", p_msg0_resp_full->size);
      printf("Response type: %d\n", p_msg0_resp_full->type);
      #endif 

      int wrote = 0;
      // wirte to dump to socket
      wrote = boost::asio::write(sock, boost::asio::buffer(
        p_msg0_resp_full, 
        sizeof(ra_samp_response_header_t) + p_msg0_resp_full->size)
      );
      #ifdef MYDEBUG 
      printf("Actual write: %d\n", wrote);
      printf("Hoped write: %d\n", sizeof(ra_samp_response_header_t) + p_msg0_resp_full->size);
      #endif 

      if((!p_msg0_resp_full->size) && (p_msg0_resp_full->type == 6)){
        gettimeofday(&t_finished,NULL);
        unsigned long m_finished = 1000000 * t_finished.tv_sec + t_finished.tv_usec;
        printf("Time connected [us] %lu, time finished [us] %lu, time diff [us] %lu, iterations %d\n", m_connected, m_finished, m_finished - m_connected, iter_counter);
        
        iter_counter = 0;


        ra_free_network_response_buffer(p_msg0_resp_full);

        exit(0);
      }

      ra_free_network_response_buffer(p_msg0_resp_full);
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
    if (argc != 4)
    {
      std::cerr << "Usage: blocking_tcp_echo_server <port>\n";
      return 1;
    }

    boost::asio::io_context io_context;


    // config 
    steps =  std::atoi(argv[2]);
    max_iterations = std::atoi(argv[3]);
    //


    server(io_context, std::atoi(argv[1]));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
