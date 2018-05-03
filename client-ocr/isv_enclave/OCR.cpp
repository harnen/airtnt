#include "sgx_trts.h"
#include "string.h"
//#include "Enclave_t.h"


#include "../isv_app/processing.h"

// test
#include <string>
#include "../isv_app/Letter.h"

#include "ocr.h"
#include "ocr_type.h"



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



/*
 * scale_Matrix_to
 * scale a 2d vector matrix A to match the size of a matrix B.
 */
vector< vector<int> > scale_Matrix_to(int width, int height, vector< vector<int> > oldMatrix) {
	// init a matrix
	vector< vector<int> > matrix;

	// compute scale factors
	double wScaler = (double) oldMatrix.size() / (double) width;
	double hScaler = (double) oldMatrix.at(0).size() / (double) height;

	// fill matrix
	for (double x = 0; x < oldMatrix.size(); x+=wScaler) {
		vector<int> column;
		for (double y = 0; y < oldMatrix.at(0).size(); y+=hScaler) {
			column.push_back(oldMatrix.at(x).at(y));
		}
		matrix.push_back(column);
	}
	return matrix;
}


/*
 * compare_matrices
 * return the distance between two matrices A and B.
 */
double compare_matrices(vector< vector<int> > matrixA, vector< vector<int> > matrixB) {
	int differenceSum = 0;

	// compute differences betwieen matrices
	for (int x = 0; x < matrixA.size(); x++) {
		for (int y = 0; y < matrixA.at(0).size(); y++) {
			if (matrixA.at(x).at(y)-matrixB.at(x).at(y) > 0) {
				differenceSum += matrixA.at(x).at(y) - matrixB.at(x).at(y);
			}
			else {
				differenceSum += matrixB.at(x).at(y) - matrixA.at(x).at(y);
			}
		}
	}

	// return the average difference between matrices A and B
	return (double) differenceSum / (double) (matrixA.size()*matrixA.at(0).size());
}








/*
void detect_letter(vector thresholdInputPixels, int* out){
	*out = 1;
}
*/


void character_recognition_wrap(ocr_input_t* ocr_input, size_t ocr_input_size, int** letters_c, int letters_rows, 
	char *output_letters, int *length) {

	//////////////////////////////////////////
	// convert input to vector
	//////////////////////////////////////////
	int rows = ocr_input->rows;
	int cols = ocr_input->cols;

	vector< vector<int> > input_image;
	input_image.resize(rows, vector<int>(cols, 0));
	for (int i=0; i < rows; i++) {
    	for (int j=0; j < cols; j++) {
        	//input_image[i][j] = input[i][j];
        	input_image[i][j] = ocr_input->payload[i*rows+j];
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

	/*ocall_print_int(input_image.size());
	ocall_print_int(input_image.at(0).size());*/

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
						int pixel = input_image.at(subx).at(suby);
						//ocall_print_int(pixel);
						column.push_back(pixel);
						//mat[subx-leftEdge][subx-topEdge] = input_image.at(subx).at(suby);
						
					}
					subMatrix.push_back(column);
				}
				//mat[x-1-leftEdge][bottomEdge-1-topEdge] = input_image.at(x-1).at(bottomEdge-1);
				

				// initialize a letter of the input_image
				Letter2 letter(subMatrix);
				letter.setX(leftEdge);
				letter.setY(topEdge);
				possible_letters.push_back(letter);

				// set values back to original state to search for more shapes
				leftEdge = -1;
				topEdge = -1;
				bottomEdge = -1;

			}
		}
	}

	//////////////////////////////////////////
    // set number of recognised letters
    //////////////////////////////////////////
	*length = possible_letters.size();
	//ocall_print_int(*length);


	
	//////////////////////////////////////////
    // find the best match in the alphabet for each letter
    //////////////////////////////////////////
	for (int i = 0; i < possible_letters.size(); i++) {
		// letter to match
		Letter2 to_match = possible_letters.at(i);

		// init the lowest known difference in letters
		double lowest_difference = 256;
		int lowest_difference_index = -1;

		// loop over each letter
		for (int j = 0; j < letters.size(); j++) {
			// scale the template letter to match the input letter then compare the matrices
			double difference = compare_matrices(
				to_match.getMatrix(),
				scale_Matrix_to(
					to_match.getMatrix().size(), 
					to_match.getMatrix().at(0).size(), 
					letters.at(j).getMatrix()
				)
			);

			// update lowest difference value and the index
			if (difference < lowest_difference) {
				lowest_difference = difference;
				lowest_difference_index = j;
			}
		}

		// get the letter with the lowest difference value
		Letter2 best_match = letters.at(lowest_difference_index);
		output_letters[i] = best_match.getLetter();
		//ocall_print_char(best_match.getLetter());
	}
	


}


