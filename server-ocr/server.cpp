#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
 
// image processing
#include <vector>
#include "Letter.h"
#include "image_util.h"
#include "processing.h"
using namespace std;

// SGX attestation
#include "network_ra.h"

// timing
#include <time.h>
#include <sys/time.h>

// OCR input structure
#include "misc.h"

#define MAX_MESSAGE_LEN 20000


/**
 *
 *
 */
int main(int argc , char *argv[]) {
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[MAX_MESSAGE_LEN];

    // read input arguments
    int port = atoi(argv[1]);
    int steps =  atoi(argv[2]);
    int max_iterations = atoi(argv[3]);
    int iter_counter=0;

    // load image
    char const *image_input = "./data/input_4_OK.png";
    vector< vector<int> > pixels;
    if (load_image(image_input, &pixels) != 0) {
        PRINT("[server] Could not load input image: %s\n", image_input);
        return -1;
    }

    // convert input to C type 
    vector<int*> ptrs;
    transform(begin(pixels), end(pixels), back_inserter(ptrs), [](vector<int> &inner_vec) {
        return inner_vec.data();
    });
    int **input =  ptrs.data();
    int rows = pixels.size();
    int cols = pixels[0].size();

    // convert to 1d array
    unsigned long ocr_input_size = sizeof(ocr_input_t) + (rows * cols * sizeof(int));
    ocr_input_t* ocr_input = (ocr_input_t*)malloc(ocr_input_size);
    ocr_input->rows = rows;
    ocr_input->cols = cols;
    for (int i=0; i < rows; ++i) {
        for (int j=0; j < cols; ++j) {
            ocr_input->payload[i*rows+j] = input[i][j];
        }
    }
    PRINT("[server] OCR Image size: %d (rows: %d, cols: %d)\n", ocr_input_size, rows, cols);
     
    // create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1) {
        PRINT("[server] Could not create socket\n");
    }
    PRINT("[server] Socket created\n");
    int option = 1;
    setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
        
    // prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
     
    // bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0) {
        //print the error message
        PRINT("[ERROR][server] bind failed\n");
        return 1;
    }
    PRINT("[server] bind done\n");
     
    // listen
    listen(socket_desc , 3);
     
    // accept and incoming connection
    PRINT("[server] Waiting for incoming connections...\n");
    c = sizeof(struct sockaddr_in);
     
    // accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0) {
        PRINT("[ERROR][server] accept failed\n");
        return 1;
    }
    PRINT("[server] Connection accepted\n");
     
    //////////////////////////////////////////
    // main loop
    // receive a message from client
    //
    //////////////////////////////////////////
    struct timeval t_connected, t_finished;
    gettimeofday(&t_connected,NULL);
    unsigned long m_connected = 1000000 * t_connected.tv_sec + t_connected.tv_usec;
    
    while(true) {
        // read header
        read_size = recv(client_sock, client_message, sizeof(ra_samp_request_header_t), 0);
        ra_samp_request_header_t *header = (ra_samp_request_header_t*) client_message;
        PRINT("[server] Received header (type: %d, size: %d)\n", header->type, header->size);
        PRINT("[server] Received header size: %d\n", read_size);
        iter_counter++;

        // exit if read zero
        if(read_size == 0) {
            PRINT("[server] Client disconnected\n");
            fflush(stdout);
            close(socket_desc); close(client_sock);
            return 0;
        }
        else if(read_size == -1) {
            PRINT("[ERROR][server] recv failed\n");
            return -1;
        } 

        // read body
        read_size = 0;
        while(read_size < header->size) {
            read_size += recv(client_sock, client_message+sizeof(ra_samp_request_header_t) + read_size, header->size - read_size, 0); 
            PRINT("[server] Received body size: %d\n", read_size);
        }
        if (header->size != read_size) {
            PRINT("[ERROR][server] Received size: %d, while expecting: %d\n", read_size, header->size);
            return -1;
        }

        // pass to SGX attestation
        ra_samp_response_header_t *p_resp_full = NULL;
        ra_network_send_receive(
            "http://example.com",
            (ra_samp_request_header_t*) client_message,
            &p_resp_full,
            steps, 
            max_iterations,
            ocr_input
        );

        // send the message back to client
        int response_size = sizeof(ra_samp_response_header_t) + p_resp_full->size;
        int wrote_size = write(client_sock , p_resp_full , response_size);
        PRINT("[server] Response size: %d\n", wrote_size);
        if (response_size != wrote_size) {
            PRINT("[ERROR][server] Response size: %d, while expecting: %d\n", wrote_size, response_size);
            return -1;
        }

        // exit & print time
        if((!p_resp_full->size) && (p_resp_full->type == 6)){
            gettimeofday(&t_finished,NULL);
            unsigned long m_finished = 1000000 * t_finished.tv_sec + t_finished.tv_usec;
            printf("Time connected [us] %lu, time finished [us] %lu, time diff [us] %lu, iterations %d\n", 
                m_connected, m_finished, m_finished - m_connected, iter_counter
            );
            
            iter_counter = 0;
            ra_free_network_response_buffer(p_resp_full);
            close(client_sock); close(socket_desc);
            exit(0);
          }

        // free
        ra_free_network_response_buffer(p_resp_full);
    }
     
    return 0;
}








