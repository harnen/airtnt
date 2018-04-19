// ============================================================================
// main.cpp
// Text recognition using matrix comparison in SFML.
//
// ============================================================================
#include <vector>
#include "Letter.h"
#include "image_util.h"
#include "processing.h"

using namespace std;

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

// ============================================================================
// main
// ============================================================================
int main() {
	// load alphabet
	char const *image_alphabet = "../data/image_alphabet.png";
	char const *text_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int const alphabet_length = 26;

	// creatre alphabet template
	// Note: this is needed only once to generate the .sav files
	int ret = create_template(image_alphabet, text_alphabet, alphabet_length);
	if (ret != 0) {
		printf("Could not load alphabet image file: %s\n", image_alphabet);
		return -1;
	}

	// load alphabet template
	// Note: this should be an OCALL from 'character_recognition'
	vector<Letter> letters;
	load_template(&letters, alphabet_length);

	// load input
	char const *image_input = "../data/input_2.png";
	vector< vector<int> > pixels;
	if (load_image(image_input, &pixels) != 0) {
		printf("Could not load input image: %s\n", image_input);
		return -1;
	}

	// convert input to C type for ECALL
	vector<int*> ptrs(pixels.size());
    transform(begin(pixels), end(pixels), begin(ptrs), [](std::vector<int> &inner_vec) {
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
		vector<int> tmp_vec (data, data + sizeof(data) / sizeof(data[0]) );
		letters_vec.push_back(tmp_vec);
	}
	vector<int*> letters_ptrs(letters_vec.size());
    transform(begin(letters_vec), end(letters_vec), begin(letters_ptrs), [](std::vector<int> &inner_vec) {
        return inner_vec.data();
    });
    int **letters_c =  letters_ptrs.data();
   	int letters_rows = letters_vec.size();
    int letters_cols = letters_vec[0].size();



    /********************** START ECALL **********************/
    // perform OCR on input
    char recognised_letters[100];
    int length;
    character_recognition(input, rows, cols, letters_c, letters_rows, letters_cols, recognised_letters, &length);

    /*********************** END ECALL ***********************/


    // print result
    printf("OCR output: ");
    for (int i = 0; i < length; i++) {
    	printf("%c", recognised_letters[i]);
    }
    printf("\n");


    // free mem
    pixels.clear();
}
