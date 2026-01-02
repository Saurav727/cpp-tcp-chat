#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>

using namespace std;

void receiveMessage(int sock) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes <= 0) break;
        std::cout << buffer << std::endl;

    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Usage: ./client <ip> <port>\n";
        return 1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(stoi(argv[2]));
    inet_pton(AF_INET, argv[1], &server.sin_addr);

    if (connect(sock, (sockaddr*)&server, sizeof(server)) < 0) {
        perror("connect");
        return 1;
    }
    
    cout << "Connected to server\n";

    std::thread recv_thread(receiveMessage, sock);
    recv_thread.detach();

    string msg;
    while (true) {
        getline(cin, msg);
        if (msg == "exit") break;
        send(sock, msg.c_str(), msg.size(), 0);
    }
    

    close(sock);
    return 0;
}
