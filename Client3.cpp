#include <iostream>
#include <thread>
#include <cstring>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

struct GamePacket
{
    int playerId;
    float posX;
    float posY;
    int health;
    int score;
    char action[20];
};

int sockfd;

void receiveData()
{
    GamePacket packet;

    while(true)
    {
        int bytes =
            recv(sockfd,
                 &packet,
                 sizeof(packet),
                 0);

        if(bytes <= 0)
            break;

        cout << "\n===== OTHER PLAYER =====\n";

        cout << "Player ID : "
             << packet.playerId
             << endl;

        cout << "Position : "
             << packet.posX
             << ", "
             << packet.posY
             << endl;

        cout << "Health : "
             << packet.health
             << endl;

        cout << "Score : "
             << packet.score
             << endl;

        cout << "Action : "
             << packet.action
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

    thread receiver(receiveData);

    GamePacket packet;

    cout << "Enter Player ID : ";
    cin >> packet.playerId;

    while(true)
    {
        cout << "\nPosition X : ";
        cin >> packet.posX;

        cout << "Position Y : ";
        cin >> packet.posY;

        cout << "Health : ";
        cin >> packet.health;

        cout << "Score : ";
        cin >> packet.score;

        cout << "Action : ";
        cin >> packet.action;

        send(sockfd,
             &packet,
             sizeof(packet),
             0);
    }

    receiver.join();

    close(sockfd);

    return 0;
}