#include "sgx_trts.h"
#include "string.h"
#include "Enclave_t.h"


#include "../lib/processing.h"

// test
#include <string>


/**
 * Test function -- Simple addition
 */
void test_function(int* a, int* b, int* c) {
    *c = *a + *b;
}
void test_function_2(const char *arr[], size_t len) {
	ocall_print("Start example function 2...");

    vector<string> v(arr, arr+len);

    ocall_print(v[0].c_str());
    ocall_print(v[1].c_str());
    ocall_print("End example function 2");
}
void test_function_3(const int *arr[], size_t rows, size_t cols) {
	ocall_print("Start example function 3...");

	vector< vector<int> > reconstruct_input;
	reconstruct_input.resize(rows, vector<int>(cols, 0));
	for (int i = 0; i < rows; ++i) {
    	for (int j = 0; j < cols; ++j) {
        	reconstruct_input[i][j] = arr[i][j];
        	ocall_print_int(reconstruct_input[i][j]);
        }
    }


    ocall_print("End example function 3");
}


/*
void detect_letter(vector thresholdInputPixels, int* out){
	*out = 1;
}
*/


void character_recognition_wrap(int** input, int rows, int cols, int** letters_c, int letters_rows, int letters_cols, 
	char *output_letters, int *length) {

	
	// convert input to vector
	vector< vector<int> > input_image;
	input_image.resize(rows, vector<int>(cols, 0));
	for (int i=0; i < rows; ++i) {
    	for (int j=0; j < cols; ++j) {
        	input_image[i][j] = input[i][j];
        }
    }
    
    // convert letters C-type to object
    //vector< vector<int> > letters_vec;
	//letters_vec.resize(letters_rows, vector<int>(letters_cols, 0));
	vector<Letter> letters;
	for (int i=0; i < letters_rows; i++) {
		int data[letters_cols];
    	for (int j=0; j < letters_cols; j++) {
    		data[j] = letters_c[i][j];

    		// import letter
    		Letter tmp = Letter::importLetter(data, sizes[i][0], sizes[i][1]);
    		// add letter to vector
    		//letters.push_back(tmp);
        	//letters_vec[i][j] = letters_c[i][j];
        }
    }

    /*
    vector<Letter> letters;
    for(int i=0; i<letters_rows; i++) {
		// import letter i
		int *data = letters_vec[i].data();
		Letter tmp = Letter::importLetter(data, sizes[i][0], sizes[i][1]);
		letters.push_back(tmp);
	}
	*/

	/*
	// find letters in image
	vector<Letter> possible_letters = find_letters(127, input_image);

	// set number of recognised letters
	*length = possible_letters.size();

	// find the best match in the alphabet for each letter
	for (int i = 0; i < possible_letters.size(); i++) {
		Letter match = letters.at(0); 
		match_letter(possible_letters.at(i), letters, &match);
		output_letters[i] = match.getLetter();
	}
	*/


}


