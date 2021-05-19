#include<iostream>
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>

char socket_path[] = "./connect4_server";

int main()
{
    struct sockaddr_un addr;
    int fd;

    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(fd == -1)
    {
        std::cout << "Socket error\n";
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);

    if(connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        std::cout << "Connect error\n";
        return -1;
    }

    while(true)
    {
        std::cout << '>';
        std::string inp;
        std::cin >> inp;

        const char* inp_cstr = inp.c_str();

        write(fd, inp_cstr, strlen(inp_cstr));

        char buf[4096];
        memset(buf, 0, sizeof(buf));
        int rc = recv(fd, buf, sizeof(buf), 0);
        if(rc == -1)
        {
            std::cout << "Receive error\n";
        }
        std::cout << buf << std::endl;
    }

    return 0;
}
