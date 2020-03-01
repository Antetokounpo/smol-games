#include "matrix.hpp"

class GOL{
    private:
        int height;
        int width;
        matrix grid;
    public:
        GOL(int h, int w);
        ~GOL(){}

        void update();
        void init();

        matrix get_grid();
        int get_height();
        int get_width();
};