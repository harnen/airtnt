// ============================================================================
// Letter.h
// Represents a matrix that is an image of a letter.
//
// ============================================================================
#include "Letter.h"


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
const vector<vector<int> >& Letter::getMatrix() const {
	return matrix;
}
void Letter::setMatrix(const vector<vector<int> >& matrix) {
	this->matrix = matrix;
}

// initialize a new letter object
Letter::Letter(vector < vector < int > > matrix) {
	this->matrix=matrix;
	this->x=0;
	this->y=0;
	this->letter='*';
}

