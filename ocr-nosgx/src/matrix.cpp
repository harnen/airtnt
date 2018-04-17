/*
 * matrix.cpp
 *
 *  Created on: May 2, 2015
 *      Author: jakebillings
 */


#include "matrix.h"






/**
 * Scale a matrix to a given height and width
 */
vector< vector < int > > scaleMatrixTo(int width, int height, vector< vector < int > > oldMatrix) {
	//Initialize a matrix
	vector< vector < int > > matrix;

	//Calculate the increments to jump in order to fill a matrix of a different size
	double wScaler = (double) oldMatrix.size() / (double) width;
	double hScaler = (double) oldMatrix.at(0).size() / (double) height;

	//Fill the new matrix
	for (double x = 0; x < oldMatrix.size(); x+=wScaler) {
		vector<int> column;
		for (double y = 0; y < oldMatrix.at(0).size(); y+=hScaler) {
			column.push_back(oldMatrix.at(x).at(y));
		}
		matrix.push_back(column);
	}

	return matrix;
}

/**
 * Used for debugging; prints a matrix to the console
 */
/*
void printMatrix (vector< vector < int > > matrix) {
	for (int x=0;x<matrix.size();x++) {
		for (int y=0;y<matrix.at(0).size();y++) {
			cout << matrix.at(x).at(y) << "\t";
		}
		cout << endl;
	}
}
*/

/**
 * Returns the average difference between matrixA and matrixB.
 * Uses the size of matrixA, so in order to get an accurate comparison, the matrices must be the same size.
 */
double compareMatrices(vector< vector < int > > matrixA, vector< vector < int > > matrixB) {
	int differenceSum = 0;

	for (int x = 0; x < matrixA.size(); x++) {
		for (int y = 0; y < matrixA.at(0).size(); y++) {
			//differenceSum+=abs(matrixA.at(x).at(y)-matrixB.at(x).at(y));
			if (matrixA.at(x).at(y)-matrixB.at(x).at(y) > 0) {
				differenceSum += matrixA.at(x).at(y)-matrixB.at(x).at(y);
			}
			else {
				differenceSum += matrixB.at(x).at(y) - matrixA.at(x).at(y);
			}
		}
	}

	//Return the average difference at any given point in matrices a and b
	return (double) differenceSum/(double) (matrixA.size()*matrixA.at(0).size());
}

/**
 * Convert a matrix to black and white using a threshold
 */
vector< vector < int > > applyThreshold(const int& threshold, const vector< vector < int > > oldMatrix, int high, int low) {
	vector< vector < int > > matrix;
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



/**
 * Scans a matrix for shapes and returns them as an array of letters
 */
vector<Letter> findLetters(const int& threshold, const vector< vector < int > > matrix) {
	//Initialize array
	vector<Letter> letters;

	//Initialize impossible base values
	int leftEdge = -1;
	int topEdge = -1;
	int bottomEdge = -1;

	for (unsigned int x = 0; x < matrix.size(); x++) {
		//If a pixel was found in this column
		bool found = false;

		for (unsigned int y = 0; y < matrix.at(0).size(); y++) {

			//Look for the top and bottom edges of the shape
			if (matrix.at(x).at(y)<threshold) {
				if (topEdge < 0 || y < topEdge) {
					topEdge = y;
				}
				found=true;
			} else {
				if (y>0&&matrix.at(x).at(y-1)<threshold&&(bottomEdge < 0 || y > bottomEdge)) {
					bottomEdge = y;
				}
			}
		}

		//Look for the left edge
		if (found) {
			if (leftEdge < 0) {
				leftEdge = x;
			}
		} else {
			if (leftEdge > 0) {
				//We've found the right edge, so we found a shape
				vector< vector < int > > subMatrix;

				//Copy the shape to a new matrix
				for (int subx = leftEdge; subx < x; subx++) {
					vector<int> column;
					for (int suby = topEdge; suby < bottomEdge; suby++) {
						column.push_back(matrix.at(subx).at(suby));
					}
					subMatrix.push_back(column);
				}

				//Initialize a letter of the matrix
				Letter letter(subMatrix);
				letter.setX(leftEdge);
				letter.setY(topEdge);
				letters.push_back(letter);

				//Set values back to original state to search for more shapes
				leftEdge = -1;
				topEdge = -1;
				bottomEdge = -1;
			}
		}
	}

	return letters;
}
