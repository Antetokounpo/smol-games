#ifndef MATRIX_H
#define MATRIX_H
#include<vector>

typedef std::vector<std::vector<int>> matrix;

matrix get_matrix(int h, int w);

void copy_matrix(matrix a, matrix b);

#endif