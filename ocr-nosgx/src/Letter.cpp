// ============================================================================
// Letter.h
// Represents a matrix that is an image of a letter.
//
// ============================================================================
#include "Letter.h"
#include <stdio.h>


// getters and setters
char Letter::getLetter() const {
	return letter;
}
void Letter::setLetter(char letter) {
	this->letter = letter;
}
int Letter::getX() const {
	return x;
}
void Letter::setX(int x) {
	this->x = x;
}
int Letter::getY() const {
	return y;
}
void Letter::setY(int y) {
	this->y = y;
}
const vector< vector<int> >& Letter::getMatrix() const {
	return matrix;
}
void Letter::setMatrix(const vector< vector<int> >& matrix) {
	this->matrix = matrix;
}

// initialize a new letter object
Letter::Letter(vector< vector<int> > matrix) {
	this->matrix=matrix;
	this->x=0;
	this->y=0;
	this->letter='*';
}

// import / export
void Letter::exportLetter(int data[]) {
	// allocate array
	//data[this->matrix.size() + this->matrix[0].size() + 3];

	// export matrix
	for (int i=0; i < matrix.size(); i++) {
		for (int j=0; j < matrix[0].size(); j++) {
			data[i+j] = matrix[i][j];
		}
	}

	// export x,y, char
	data[matrix.size() + matrix[0].size() + 0] = x;
	data[matrix.size() + matrix[0].size() + 1] = y;
	data[matrix.size() + matrix[0].size() + 2] = letter;
}

static Letter importLetter(const int *data, const int rows, const int cols) {
	// set matrix
	vector< vector<int> > new_matrix;
	for (int i=0; i < rows; i) {
		for (int j=0; j < cols; j++) {
			new_matrix[i][j] = data[i+j];
		}
	}
	Letter letter(new_matrix);

	// import x,y, char
	letter.setX(data[rows+cols+0]);
	letter.setY(data[rows+cols+1]);
	letter.setLetter(data[rows+cols+2]);

	return letter;
}




