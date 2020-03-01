#include<vector>
#include<iostream>
#include<string>
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>

class Board
{
    public:
        Board();
        ~Board();
        void clear();
        bool drop(int column, int color);
        std::vector<std::vector<int>> getBoard();
        int checkWinner();

    private:
        std::vector<std::vector<int>> columns;
};

Board::Board() : columns(7, std::vector<int>(6, 2))
{}

Board::~Board()
{}

void Board::clear()
{
    for(std::vector<std::vector<int>>::iterator it = columns.begin(); it != columns.end(); ++it)
    {
        std::fill(it->begin(), it->end(), 2);
    }
}

bool Board::drop(int column, int color)
{
    for(std::vector<int>::reverse_iterator it = columns[column].rbegin(); it != columns[column].rend(); ++it)
    {
        if(*it == 2)
        {
            *it = color;
            return true;
        }
    }

    return false;
}

std::vector<std::vector<int>> Board::getBoard()
{
    return columns;
}

int Board::checkWinner()
{
    // Horizontal
    for(int i = 0; i<columns[0].size(); ++i)
    {
        int previous = columns[0][i];
        int c = 0;
        for(int j = 1; j<columns.size(); ++j)
        {
            if(columns[j][i] == previous && previous != 2)
            {
                ++c;
                if(c == 3)
                    return previous;
            }else
            {
                c = 0;
            }
            previous = columns[j][i];
        }
    }

    // Vertical
    for(int j = 0; j<columns.size(); ++j)
    {
        int previous = columns[j][0];
        int c = 0;
        for(int i = 1; i<columns[0].size(); ++i)
        {
            if(columns[j][i] == previous && previous != 2)
            {
                ++c;
                if(c == 3)
                    return previous;
            }else
            {
                c = 0;
            }
            previous = columns[j][i];
        }
    }

    // Diagonal vers la droite en descendant, partant de j=0
    for(int d = 2; d>=0; d--)
    {
        int previous = columns[0][d];
        int c = 0;
        for(int j = 1, i = d+1; i<6; j++, i++)
        {
            if(columns[j][i] == previous && previous != 2)
            {
                ++c;
                if(c == 3)
                    return previous;
            }else
            {
                c = 0;
            }
            previous = columns[j][i];
        }
    }

    // Diagonal vers la droite en descendant, partant de i=0
    for(int d = 1; d<=3; d++)
    {
        int previous = columns[d][0];
        int c = 0;
        for(int j = d+1, i = 1; j<7; j++, i++)
        {
            if(columns[j][i] == previous && previous != 2)
            {
                ++c;
                if(c == 3)
                    return previous;
            }else
            {
                c = 0;
            }
            previous = columns[j][i];
        }
    }

    for(int d = 3; d<=5; d++)
    {
        int previous = columns[d][0];
        int c = 0;
        for(int j = d-1, i = 1; j>=0; j--, i++)
        {
            if(columns[j][i] == previous && previous != 2)
            {
                ++c;
                if(c == 3)
                    return previous;
            }else
            {
                c = 0;
            }
            previous = columns[j][i];
        }
    }

    for(int d = 0; d<=2; d++)
    {
        int previous  = columns[6][d];
        int c = 0;
        for(int j = 5, i=d+1; i<6; j--, i++)
        {
            if(columns[j][i] == previous && previous != 2)
            {
                ++c;
                if(c == 3)
                    return previous;
            }else
            {
                c = 0;
            }
            previous = columns[j][i];
        }
    }

    return -1;
}

int main()
{
    Board board;
    std::vector<sf::Color> colors {sf::Color::Red, sf::Color::Yellow, sf::Color::White};
    int turn = 0;

    sf::Font font;
    if(!font.loadFromFile("roboto.ttf"))
    {
        std::cout << "Error" << std::endl;
    }
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Regular);

    sf::RenderWindow win(sf::VideoMode(700, 600), "Connect 4");
    win.setKeyRepeatEnabled(false);
    while(win.isOpen())
    {
        sf::Event e;
        while(win.pollEvent(e))
        {
            switch(e.type)
            {
                case sf::Event::MouseButtonPressed:
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        sf::Vector2i position = sf::Mouse::getPosition(win);
                        if(board.drop(position.x/100, turn%2))
                        {
                            turn++;
                        }
                    }
                    break;
                
                case sf::Event::Closed:
                    win.close();
                    break;
            }
        }
        win.clear(sf::Color::Blue);
        
        //std::cout << turn%2 << std::endl;

        text.setString("Turn "+std::to_string(turn%2+1));

        if(board.checkWinner() >= 0)
        {
            std::cout << "Winner: " + std::to_string(board.checkWinner()) << std::endl;
            return -1;
        }

        std::vector<std::vector<int>> game_board = board.getBoard();
        for(int i = 0; i<7; ++i)
        {
            for(int j = 0; j<6; ++j)
            {
                sf::CircleShape circle(50);
                circle.setFillColor(colors[game_board[i][j]]);
                circle.setPosition(100*i, 100*j);
                win.draw(circle);
            }
        }

        win.draw(text);

        win.display();
    }
    return 0;
}