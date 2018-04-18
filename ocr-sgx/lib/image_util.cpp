// ============================================================================
// image_util.cpp
// Image utils relying on the SFML graphic library.
//
// ============================================================================
#include "image_util.h"


/*
 * load_alphabet
 * load an image-database file and a text cile containing all letters in the alphabet.
 */
int load_alphabet(const char *image_alphabet, const char *text_alphabet, vector<Letter> *letters) {
	// load the databse image file - image file containing evey letters
	vector< vector<int> > pixels;
	if (load_image(image_alphabet, &pixels) != 0) {return 1;}

	// locate all the 'letters' in the image file
	*letters = find_letters(127, pixels);

	// open the text file
	ifstream data(text_alphabet);
	if (data.fail()) {return 2;}

	// read each space-delimited letter into its corresponding letter object
	int i = 0;
	while (!data.eof()&&i<(*letters).size()) {
		char c;
		data >> c;
		(*letters).at(i++).setLetter(c);
	}
	return 0;
}


/*
 * load_image
 * load an image into a 2d vector.
 */
int load_image(const char *image_file, vector< vector<int> > *pixels) {
	// load image
	Image input_image;
	if (!input_image.loadFromFile(image_file)) {return 1;}

	// apply the threshold to the input text pixels
	*pixels = apply_threshold(THRESHOLD, get_grayscale_pixels(input_image), 255, 0);
	return 0;
}


/*
 * get_grayscale_pixels
 * convert an image into a grayscale 2d vector.
 */
vector< vector<int> > get_grayscale_pixels(const Image& img) {
	vector< vector<int> > matrix;

	for (int x = 0; x < img.getSize().x; x++) {
		vector<int> column;
		for (int y = 0; y < img.getSize().y; y++) {
			Color c = img.getPixel(x,y);
			unsigned int shade = RED_WEIGHT * c.r + GREEN_WEIGHT * c.g + BLUE_WEIGHT * c.b;
			column.push_back(shade);
		}
		matrix.push_back(column);
	}
	return matrix;
}


/*
 * apply_threshold
 * apply a threshold to a 2d vector representing a grayscale image.
 */
vector< vector <int> > apply_threshold(const int& threshold, const vector< vector <int> > oldMatrix, int high, int low) {
	vector< vector <int> > matrix;

	// apply threshold at every point
	for (int x = 0; x < oldMatrix.size(); x++) {
		vector<int> column;
		for (int y = 0; y < oldMatrix.at(0).size(); y++) {
			unsigned int shade = oldMatrix.at(x).at(y) > threshold ? high : low;
			column.push_back(shade);
		}
		matrix.push_back(column);
	}
	return matrix;
}
