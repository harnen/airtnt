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


#include <fstream>
#include <string>

// ============================================================================
// main
// ============================================================================
int main() {

	// load alphabet
	char const *image_alphabet = "../data/image_alphabet.png";
	char const *text_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int const alphabet_length = 26;





	// test
	ifstream fin(image_alphabet, std::ios::binary);
	string data((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	//printf("%s\n", data);






	vector<Letter> letters;
	int ret = load_alphabet(image_alphabet, text_alphabet, alphabet_length, &letters);
	if (ret != 0) {
		printf("Could not load alphabet image file: %s\n", image_alphabet);
		return -1;
	}


	// load input
	char const *image_input = "../data/input_2.png";
	vector<vector<int>> pixels;
	if (load_image(image_input, &pixels) != 0) {
		printf("Could not load input image: %s\n", image_input);
		return -1;
	}


	// convert to C type for Ecall
	vector<int*> ptrs(pixels.size());
    transform(begin(pixels), end(pixels), begin(ptrs), [](std::vector<int> &inner_vec) {
        return inner_vec.data();
    });
    int **input =  ptrs.data();
   	int rows = pixels.size();
    int cols = pixels[0].size();


    /********************** START ECALL **********************/
    // perform OCR on input
    char recognised_letters[100];
    int length;
    character_recognition(input, rows, cols, letters, recognised_letters, &length);

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
