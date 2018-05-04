#ifndef OCR_H_
#define OCR_H_


#include "string.h"
#include "../isv_app/processing.h"
#include "../isv_app/Letter.h"


#include "ocr_type.h"



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




vector< vector<int> > scale_Matrix_to(int width, int height, vector< vector<int> > oldMatrix);
double compare_matrices(vector< vector<int> > matrixA, vector< vector<int> > matrixB);
void character_recognition_wrap(ocr_input_t* ocr_input, size_t ocr_input_size, int** letters_c, int letters_rows, 
	char *output_letters, int *length);




#endif // OCR_H_
