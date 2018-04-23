#include "sgx_trts.h"
#include "string.h"
#include "Enclave_t.h"


#include "../../lib/processing.h"

// test
#include <string>
#include "../../lib/Letter.h"



class Letter2 {
private:
	/**
	 * The char ascii value of the character represented by this letter class.  Defaults to * if
	 * the character is unknown (for instance, in the case of unscanned letters)
	 */
	char letter;

	/**
	 * A matrix containing
	 */
	vector< vector<int> > matrix;

	/**
	 * The x and y coordinates in the parent matrix of where the matrix was found
	 */
	int x;
	int y;

public:
	//Initialize a new letter from a matrix
	Letter2(vector< vector<int> > matrix);

	//Getters and setters
	char getLetter() const;
	void setLetter(char letter);
	const vector<vector<int> >& getMatrix() const;
	void setMatrix(const vector<vector<int> >& matrix);

	int getX() const;
	void setX(int x);
	int getY() const;
	void setY(int y);

	void exportLetter(int data[]);
	static Letter2 importLetter(const int data[], const int rows, const int cols);
};

// getters and setters
char Letter2::getLetter() const {
	return letter;
}
void Letter2::setLetter(char letter) {
	this->letter = letter;
}
int Letter2::getX() const {
	return x;
}
void Letter2::setX(int x) {
	this->x = x;
}
int Letter2::getY() const {
	return y;
}
void Letter2::setY(int y) {
	this->y = y;
}
const vector< vector<int> >& Letter2::getMatrix() const {
	return matrix;
}
void Letter2::setMatrix(const vector< vector<int> >& matrix) {
	this->matrix = matrix;
}

// initialize a new letter object
Letter2::Letter2(vector< vector<int> > matrix) {
	this->matrix=matrix;
	this->x=0;
	this->y=0;
	this->letter='*';
}

// import / export
void Letter2::exportLetter(int data[]) {
	// allocate array
	//data[this->matrix.size() + this->matrix[0].size() + 3];

	// export matrix
	int rows = matrix.size();
	int cols = matrix[0].size();
	for (int i=0; i < rows; i++) {
		for (int j=0; j < cols; j++) {
			data[cols*i+j] = matrix[i][j];
		}
	}

	// export x,y, char
	data[matrix.size() * matrix[0].size() + 0] = x;
	data[matrix.size() * matrix[0].size() + 1] = y;
	data[matrix.size() * matrix[0].size() + 2] = (int) letter;
}

Letter2 Letter2::importLetter(const int data[], const int rows, const int cols) {
	// set matrix
	vector< vector<int> > new_matrix;
	new_matrix.resize(rows, vector<int>(cols, 0));
	for (int i=0; i < rows; i++) {
		for (int j=0; j < cols; j++) {
			new_matrix[i][j] = data[cols*i+j];
		}
	}
	Letter2 letter(new_matrix);

	// import x,y, char
	letter.setX(data[rows * cols + 0]);
	letter.setY(data[rows * cols + 1]);
	letter.setLetter((char) data[rows * cols + 2]);

	return letter;
}






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


    ocall_print("End example function 3\n\n");
}


/*
void detect_letter(vector thresholdInputPixels, int* out){
	*out = 1;
}
*/


void character_recognition_wrap(int** input, int rows, int cols, int** letters_c, int letters_rows, 
	char *output_letters, int *length) {

	//////////////////////////////////////////
	// convert input to vector
	//////////////////////////////////////////
	vector< vector<int> > input_image;
	input_image.resize(rows, vector<int>(cols, 0));
	for (int i=0; i < rows; i++) {
    	for (int j=0; j < cols; j++) {
        	input_image[i][j] = input[i][j];
        }
    }
    

    //////////////////////////////////////////
    // convert letters C-type to object
    //////////////////////////////////////////
    vector<Letter2> letters;
	for (int i = 0; i < letters_rows; i++) {

		int letters_cols = sizes[i][0] * sizes[i][1] + 3;
		int data[letters_cols];
    	for (int j = 0; j < letters_cols; j++) {
        	data[j] = letters_c[i][j];
        }
        Letter2 retrieved_letter = Letter2::importLetter(data,sizes[i][0], sizes[i][1]);
        letters.push_back(retrieved_letter);

        // test: debug
        ocall_print_int(retrieved_letter.getLetter());
    }


    //////////////////////////////////////////
    // find letters in image
    //////////////////////////////////////////
    int threshold = 127;
    vector<Letter2> possible_letters;
	// initialize impossible base values
	int leftEdge = -1;
	int topEdge = -1;
	int bottomEdge = -1;

	for (unsigned int x = 0; x < input_image.size(); x++) {
		// determin whether a pixel was found in this column
		bool found = false;

		for (unsigned int y = 0; y < input_image.at(0).size(); y++) {
			// look for the top and bottom edges of the shape
			if (input_image.at(x).at(y) < threshold) {
				if (topEdge < 0 || y < topEdge) {topEdge = y;}
				found = true;
			} 
			else {
				if (y>0 && input_image.at(x).at(y-1) < threshold && (bottomEdge<0 || y>bottomEdge)) {
					bottomEdge = y;
				}
			}
		}

		// look for the left edge
		if (found) {
			if (leftEdge < 0) {leftEdge = x;}
		} 
		else {
			if (leftEdge > 0) {
				// we've found the right edge, so we found a shape
				vector< vector<int> > subMatrix;
				//int mat[x-leftEdge][bottomEdge-topEdge];

				// copy the shape to a new input_image
				for (int subx = leftEdge; subx < x; subx++) {
					vector<int> column;
					for (int suby = topEdge; suby < bottomEdge; suby++) {
						column.push_back(input_image.at(subx).at(suby));
						//mat[subx-leftEdge][subx-topEdge] = input_image.at(subx).at(suby);
						
					}
					subMatrix.push_back(column);
				}

				//mat[x-1-leftEdge][bottomEdge-1-topEdge] = input_image.at(x-1).at(bottomEdge-1);
				ocall_print_int(x);
				/*
				// initialize a letter of the input_image
				Letter2 letter(subMatrix);
				letter.setX(leftEdge);
				letter.setY(topEdge);
				possible_letters.push_back(letter);

				// set values back to original state to search for more shapes
				leftEdge = -1;
				topEdge = -1;
				bottomEdge = -1;
				*/
			}
		}
	}



	


}


