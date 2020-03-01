#include<SFML/Graphics.hpp>
#include "gol.hpp"
#include "matrix.hpp"

#define HEIGHT 600
#define WIDTH 600

int main(){
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Game of Life");
    GOL gol(HEIGHT, WIDTH);
    sf::VertexArray point(sf::Points, HEIGHT*WIDTH);
    gol.init();

    for(int i = 0; i<HEIGHT; i++){
        for(int j = 0; j<WIDTH; j++){
            point[i*WIDTH+j].position = sf::Vector2f(i, j);
            point[i*WIDTH+j].color = sf::Color::Black;
        }
    }

    while(window.isOpen()){
        sf::Event e;
        while(window.pollEvent(e)){
            if(e.type == sf::Event::Closed)
                window.close();
        }
        
        window.clear(sf::Color::Black);
        gol.update();
        matrix map = gol.get_grid();

        for(int i = 0; i<HEIGHT; i++){
            for(int j = 0; j<WIDTH; j++){
                if(map[i][j] == 1)
                    point[i*WIDTH+j].color = sf::Color::White;
                else
                    point[i*WIDTH+j].color = sf::Color::Black;
                
            }
        }

        window.draw(point);        

        window.display();
    }
    return 0;
}