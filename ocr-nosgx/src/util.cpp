#include "util.h"


void image2ptrs(const vector<vector<int>> image, int ***ptrs) {
	vector<int*> tmp(image.size());
    transform(begin(image), end(image), begin(tmp), [](std::vector<int> &inner_vec) {
        return inner_vec.data();
    });
    *ptrs = tmp.data();
}


void ptrs2image(const int **ptrs, const int rows, const int cols, vector<vector<int>> *image) {
	(*image).resize(rows, vector<int>(cols, 0));

	for (int i = 0; i < rows; ++i) {
    	for (int j = 0; j < cols; ++j) {
        	(*image)[i][j] = ptrs.data()[i][j];
        }
    }
}