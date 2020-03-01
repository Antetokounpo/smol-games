#include "matrix.hpp"
#include<vector>

matrix get_matrix(int h, int w){
    return std::vector<std::vector<int>>(h, std::vector<int>(w));
}

void copy_matrix(matrix a, matrix b){
    for(int i = 0; i<a.size(); i++){
        for(int j = 0; j<a[i].size(); i++){
            b[i][j] = a[i][j];
        }
    }
}