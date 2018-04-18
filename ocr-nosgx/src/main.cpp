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

	
	/*
	vector<Letter> letters;
	int ret = load_alphabet(image_alphabet, text_alphabet, alphabet_length, &letters);
	if (ret != 0) {
		printf("Could not load alphabet image file: %s\n", image_alphabet);
		return -1;
	}
	*/

	// creatre alphabet template
	int ret = create_template(image_alphabet, text_alphabet, alphabet_length);
	if (ret != 0) {
		printf("Could not load alphabet image file: %s\n", image_alphabet);
		return -1;
	}

	// load alphabet template
	vector<Letter> letters;
	load_template(&letters, alphabet_length);






	/********************* START TESTS ***********************/
	// read image in binary format
	//ifstream fin(image_alphabet, std::ios::binary);
	//string data((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());

	


	/*
	vector<Letter> letters_from_import;
	for(int i=0; i<letters.size(); i++) {
		// get letter matrix
		vector< vector<int> > matrix = letters[i].getMatrix();
		int data_length = matrix.size() * matrix[0].size() + 3;
	
		// export
		int data[data_length];
		letters[i].exportLetter(data);

		// import
		int const rows = matrix.size();
		int const cols = matrix[0].size();
		Letter tmp = Letter::importLetter(data, rows, cols);
		//printf("{%d, %d}, ", rows, cols);
		letters_from_import.push_back(tmp);
	}
	*/

	// save alphabet to file
	/*
	ofstream myfile;
	char filename[6];
	for(int i=0; i<letters.size(); i++) {
		// open file
		sprintf(filename, "%d.sav", i);
		myfile.open(filename);

		// save data
		vector< vector<int> > matrix = letters[i].getMatrix();
		int data_length = matrix.size() * matrix[0].size() + 3;
		int data[data_length];
		letters[i].exportLetter(data);
		
		for(int i=0; i<data_length; i++) {
			myfile << (int)data[i] << " ";
			//if (i == data_length-1) {printf("%c\n", data[i]);}
		}
		myfile.close();
	}
	*/

	

	// read alphabet from file
	/*
	char filename[6];
	vector<Letter> letters_from_file;
	for(int i=0; i<26; i++) {
		sprintf(filename, "%d.sav", i);
		ifstream fin(filename);
			
		// convert string to char[]
		int const rows = sizes[i][0];
		int const cols = sizes[i][1];
		int data[rows * cols + 3];

		// read file
		int j = 0;
		int x;
		while (fin >> x) {
			data[j++] = x;
		}
		//printf("%c\n", data[rows * cols + 2]);

		// import
		Letter tmp = Letter::importLetter(data, rows, cols);
		letters.push_back(tmp);
		fin.close();
	}
	*/
	



	/*********************** END TESTS ***********************/






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
