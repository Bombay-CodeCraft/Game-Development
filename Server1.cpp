#include <iostream>
#include <thread>
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

int clients[2];

void relay(int sender)
{
    GamePacket packet;

    while(true)
    {
        int bytes =
            recv(clients[sender],
                 &packet,
                 sizeof(packet),
                 0);

        if(bytes <= 0)
        {
            cout << "Client "
                 << sender + 1
                 << " Disconnected\n";

            break;
        }

        cout << "\n===== GAME DATA =====\n";
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

        send(clients[1 - sender],
             &packet,
             sizeof(packet),
             0);
    }
}

int main()
{
    int serverSocket;

    sockaddr_in serverAddr;

    serverSocket =
        socket(AF_INET,
               SOCK_STREAM,
               0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket,
         (sockaddr*)&serverAddr,
         sizeof(serverAddr));

    listen(serverSocket, 2);

    cout << "Waiting Client 1...\n";

    clients[0] =
        accept(serverSocket,
               NULL,
               NULL);

    cout << "Client 1 Connected\n";

    cout << "Waiting Client 2...\n";

    clients[1] =
        accept(serverSocket,
               NULL,
               NULL);

    cout << "Client 2 Connected\n";

    cout << "\nGame Communication Started\n";

    thread t1(relay, 0);
    thread t2(relay, 1);

    t1.join();
    t2.join();

    close(clients[0]);
    close(clients[1]);
    close(serverSocket);

    return 0;
}