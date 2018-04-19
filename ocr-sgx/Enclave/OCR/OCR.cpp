#include "sgx_trts.h"
#include "string.h"
#include "Enclave_t.h"


#include "../lib/processing.h"



/**
 * Test function -- Simple addition
 */
void test_function(int* a, int* b, int* c) {
    *c = *a + *b;
}

/*
void detect_letter(vector thresholdInputPixels, int* out){
	*out = 1;
}
*/


void character_recognition_wrap(int** input, int rows, int cols, int** letters_c, int letters_rows, int letters_cols, 
	char *output_letters, int *length) {

	std::vector<Letter> v;

	// lib call
	//int length_tmp;
	character_recognition(input, rows, cols, letters_c, letters_rows, letters_cols, output_letters, length);
	//*length = length_tmp;


}


