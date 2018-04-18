#include "sgx_trts.h"
#include "string.h"
#include "Enclave_t.h"

#include <vector>
#include "../lib/Letter.h"



/**
 * Test function -- Simple addition
 */
void test_function(int* a, int* b, int* c) {
    *c = *a + *b;

    vector<Letter> input_letter;
}

/*
void detect_letter(vector thresholdInputPixels, int* out){
	*out = 1;
}
*/


