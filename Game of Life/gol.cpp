#include<iostream>
#include<stdlib.h>
#include "matrix.hpp"
#include "gol.hpp"

GOL::GOL(int h, int w){
    height = h;
    width = w;

    grid = get_matrix(height, width);
}

void GOL::init(){
    for(int i = 1; i<height-1; i++){
        for(int j = 1; j<width-1; j++){
            grid[i][j] = rand()%2;
        }
    }
}

void GOL::update(){
    matrix new_grid = get_matrix(height, width);

    for(int i = 1; i<height-1; i++){
        for(int j = 1; j<width-1; j++){
            int c = 0;
            int current = grid[i][j];
            for(int _i = i - 1; _i<i+2; _i++){
                for(int _j = j - 1; _j<j+2; _j++){
                    if(grid[_i][_j])
                        c += 1;
                }
            }
            if(c < 3 && current)
                new_grid[i][j] = 0;
            else if(c > 4 && current)
                new_grid[i][j] = 0;
            else if(c == 3 && !current)
                new_grid[i][j] = 1;
            else
                new_grid[i][j] = current;
        }
    }
    grid = new_grid;
}

matrix GOL::get_grid(){
    return grid;
}

int GOL::get_height(){
    return height;
}
int GOL::get_width(){
    return width;
}

