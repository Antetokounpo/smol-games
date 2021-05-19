#include<vector>
#include<iostream>
#include<string>
#include<cstring>
#include<sys/socket.h>
#include<sys/un.h>
#include<sys/unistd.h>
#include<poll.h>
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>

static const std::string turn_colors[2] = {"RED", "YELLOW"};

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

class Connect4Server
{
    public:
        Connect4Server();
        ~Connect4Server();

        void start(Board& board);
        bool play(Board& board);
    private:
        int fd, cl, rc;
        struct pollfd fds;
        const char* socket_path = "./connect4_server";
        int timeoutval = 200;
};

Connect4Server::Connect4Server()
{
    fd = socket(AF_UNIX, SOCK_STREAM, 0);

    fds.events = POLLIN;

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
    unlink(socket_path);

    if(bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        std::cout << "Bind error\n";
    }
}

Connect4Server::~Connect4Server() {}

void Connect4Server::start(Board& board)
{
    if(listen(fd, 5) == -1)
    {
        std::cout << "Listen error\n";
        return;
    }

    cl = accept(fd, NULL, NULL);

    if(cl == -1)
    {
        std::cout << "Accept error\n";
        return;
    }

    // Set le fd pour le call de poll dans la fonction play
    fds.fd = cl;
}

bool Connect4Server::play(Board& board)
{
    while(true)
    {
        char buf[4096];
        memset(buf, 0, sizeof(buf));

        // Timeout après n millisecondes
        int rv = poll(&fds, 1, timeoutval);
        if(rv == 0)
        {
            break;
        }

        rc = recv(cl, buf, sizeof(buf), 0);
        if(rc == -1)
        {
            std::cout << "Receive error\n";
            return false;
        }

        std::string str = buf;

        if(str == "get")
        {
            std::string board_str = "";
            for(auto col : board.getBoard())
            {
                for(auto& p : col)
                {
                    board_str += std::to_string(p);
                }
                board_str += '\n';
            }
            const char* board_cstr = board_str.c_str();
            if(send(cl, board_cstr, strlen(board_cstr), 0) == -1)
            {
                std::cout << "Sending error\n";
                return false;
            }
        }
        else if (str == "exit")
        {
            const char* msg = "Cancelling...";
            send(cl, msg, strlen(msg), 0);
            break;
        }
        else
        {
            const char* err = "Wrong command";
            if(send(cl, err, strlen(err), 0) == -1)
            {
                std::cout << "Sending error\n";
                return false;
            }
        }
    }

    return false;
}


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
            }
            else
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
            }
            else
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
            }
            else
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
            }
            else
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
            }
            else
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
            }
            else
            {
                c = 0;
            }
            previous = columns[j][i];
        }
    }

    return -1;
}

sf::Text init_text()
{
    static sf::Font font;
    if(!font.loadFromFile("fonts/roboto.ttf"))
    {
        std::cout << "Error" << std::endl;
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Regular);

    return text;
}

int main(int argc, char* argv[])
{
    bool computer = false;
    if(argc > 1)
    {
        if(strcmp(argv[1], "comp") == 0)
            computer = true;
    }

    Board board;
    Connect4Server server;
    std::vector<sf::Color> colors {sf::Color::Red, sf::Color::Yellow, sf::Color::White};
    int turn = 0;
    bool over = false;
    sf::Text text = init_text();

    if(computer)
    {
        std::cout << "Waiting for client to connect...\n"; 
        server.start(board);
        std::cout << "Client connected!\n";
    }

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow win(sf::VideoMode(700, 600), "Connect 4", sf::Style::Default, settings);
    win.setKeyRepeatEnabled(false);



    while(win.isOpen())
    {
        sf::Event e;
        while(win.pollEvent(e))
        {
            switch(e.type)
            {
                case sf::Event::MouseButtonPressed:
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !over)
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

        if(!over)
            text.setString("Turn "+turn_colors[turn%2]);
        else
            text.setString(turn_colors[board.checkWinner()]+" won in "+std::to_string(turn)+" turns");

        if(board.checkWinner() >= 0)
        {
            std::cout << "Winner: " + std::to_string(board.checkWinner()) << std::endl;
            over = true;
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
        if(computer && turn % 2)
        {
            bool t = server.play(board);
            if(t) turn++;
        }
    }
    return 0;
}
