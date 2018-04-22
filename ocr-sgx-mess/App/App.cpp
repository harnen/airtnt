#include <stdio.h>
#include <iostream>
#include "Enclave_u.h"
#include "sgx_urts.h"
#include "sgx_utils/sgx_utils.h"


#include <vector>
#include "Letter.h"
#include "image_util.h"
#include "processing.h"


#include <string>

//using namespace std;


// Global EID shared by multiple threads
sgx_enclave_id_t global_eid = 0;


/* 
 * OCall implementations
 */
void ocall_print(const char* str) {
    printf("%s\n", str);
}
void ocall_print_int(int data) {
    printf("%d\n", data);
}


/* 
 * Main entry point
 */
int main(int argc, char const *argv[]) {

    
    /*****************************
     * init enclave
     ****************************/
    if (initialize_enclave(&global_eid, "enclave.token", "enclave.signed.so") < 0) {
        std::cout << "Fail to initialize enclave." << std::endl;
        return 1;
    }


    /*****************************
     * test
     ****************************/
    int ptr;

    // test enclave call
    sgx_status_t status = generate_random_number(global_eid, &ptr);
    std::cout << status << std::endl;
    if (status != SGX_SUCCESS) {
        std::cout << "noob" << std::endl;
    }
    printf("Random number: %d\n", ptr);

    // test enclave lib call
    size_t sealed_size = sizeof(sgx_sealed_data_t) + sizeof(ptr);
    uint8_t* sealed_data = (uint8_t*)malloc(sealed_size);

    sgx_status_t ecall_status;
    status = seal(global_eid, &ecall_status,
            (uint8_t*)&ptr, sizeof(ptr),
            (sgx_sealed_data_t*)sealed_data, sealed_size);

    if (!is_ecall_successful(status, "Sealing failed :(", ecall_status)) {
        return 1;
    }

    int unsealed;
    status = unseal(global_eid, &ecall_status,
            (sgx_sealed_data_t*)sealed_data, sealed_size,
            (uint8_t*)&unsealed, sizeof(unsealed));

    if (!is_ecall_successful(status, "Unsealing failed :(", ecall_status)) {
        return 1;
    }

    std::cout << "Seal round trip success! Receive back " << unsealed << std::endl;


    // test simple function
    int a = 5, b = 7;
    int c;
    test_function(global_eid, &a, &b, &c);
    printf("%d + %d = %d\n", a, b, c);


    // test function 2
    vector<string> members_data;
    members_data.push_back("apple");
    members_data.push_back("orange"); //just for sample

    vector<const char*>  vc;
    transform(members_data.begin(), members_data.end(), back_inserter(vc), [](string &s) {
        return s.c_str();
    });

    test_function_2(global_eid, vc.data(), members_data.size());

    // test function 3
    vector< vector<int> > v3 = {{1,2,3},{4,5,6}};
    vector<const int*> internal_v3;
    transform(begin(v3), end(v3), back_inserter(internal_v3), [](vector<int> &inner_vec) {
        return inner_vec.data();
    });
    test_function_3(global_eid, internal_v3.data(), v3.size(), v3[0].size());



    /*****************************
     * OCR
     ****************************/
    char const *image_alphabet = "./data/image_alphabet.png";
    char const *text_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int const alphabet_length = 26;

    
    // creatre alphabet template
    // Note: this is needed only once to generate the .sav files
    /*
    int ret = create_template(image_alphabet, text_alphabet, alphabet_length);
    if (ret != 0) {
        printf("Could not load alphabet image file: %s\n", image_alphabet);
        return -1;
    }
    */

    

    // load alphabet template
    // Note: this should be an OCALL from 'character_recognition'
    vector<Letter> letters;
    load_template(&letters, alphabet_length);

    // load input
    char const *image_input = "./data/input_2.png";
    vector< vector<int> > pixels;
    if (load_image(image_input, &pixels) != 0) {
        printf("Could not load input image: %s\n", image_input);
        return -1;
    }
    
    // convert input to C type for ECALL
    vector<int*> ptrs;
    transform(begin(pixels), end(pixels), back_inserter(ptrs), [](vector<int> &inner_vec) {
        return inner_vec.data();
    });
    int **input =  ptrs.data();
    int rows = pixels.size();
    int cols = pixels[0].size();
    
    // convert alphabet letters to C type for ECALL
    vector< vector<int> > letters_vec;
    for(int i=0; i<letters.size(); i++) {
        // export letter i
        vector< vector<int> > matrix = letters[i].getMatrix();
        int data_length = matrix.size() * matrix[0].size() + 3;
        int data[data_length];
        letters[i].exportLetter(data);
        
        // save letter
        int data_size = sizeof(data) / sizeof(data[0]);
        vector<int> tmp_vec;
        copy(&data[0], &data[data_size], back_inserter(tmp_vec));
        //vector<int> tmp_vec (data,  data + sizeof(data) / sizeof(data[0]));
        letters_vec.push_back(tmp_vec);
    }
    vector<int*> letters_ptrs;
    transform(begin(letters_vec), end(letters_vec), back_inserter(letters_ptrs), [](vector<int> &inner_vec) {
        return inner_vec.data();
    });
    int **letters_c =  letters_ptrs.data();
    int letters_rows = letters_vec.size();



    /********************** START ECALL **********************/
    // perform OCR on input
    char recognised_letters[100]; // make array big enough
    int length;
    character_recognition_wrap(global_eid, input, rows, cols, letters_c, letters_rows, recognised_letters, &length);

    /*********************** END ECALL ***********************/


    // print result
    printf("\nOCR output: ");
    for (int i = 0; i < length; i++) {
        printf("%c", recognised_letters[i]);
    }
    printf("\n");


    // free mem
    pixels.clear();



    
    /*****************************
     * destroy enclave
     ****************************/
    // Note: destroy enclave (TODO)
    return 0;
}
