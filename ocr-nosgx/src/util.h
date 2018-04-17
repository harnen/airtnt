#ifndef EUTIL_H_
#define EUTIL_H_

#include <vector>

using namespace std;

void image2ptrs(const vector<vector<int>> image, int ***ptrs);
void ptrs2image(const int **ptrs, const int rows, const int cols, vector<vector<int>> *image);

#endif // EUTIL_H_
