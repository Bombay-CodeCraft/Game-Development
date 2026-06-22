#include <iostream>
#include <thread>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int sockfd;

void receiveMessages()
{
    char buffer[1024];

    while(true)
    {
        int bytes =
            recv(sockfd,
                 buffer,
                 sizeof(buffer)-1,
                 0);

        if(bytes <= 0)
            break;

        buffer[bytes] = '\0';

        cout << "\nMessage : "
             << buffer
             << endl;
    }
}

int main()
{
    sockaddr_in serverAddr;

    sockfd =
        socket(AF_INET,
               SOCK_STREAM,
               0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);

    inet_pton(AF_INET,
              "127.0.0.1",
              &serverAddr.sin_addr);

    connect(sockfd,
            (sockaddr*)&serverAddr,
            sizeof(serverAddr));

    thread receiver(receiveMessages);

    string msg;

    while(true)
    {
        getline(cin,msg);

        if(msg == "exit")
            break;

        send(sockfd,
             msg.c_str(),
             msg.size(),
             0);
    }

    close(sockfd);

    receiver.detach();

    return 0;
}