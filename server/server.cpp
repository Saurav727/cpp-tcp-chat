#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <string>


std::vector<int> clients;
std::mutex clients_mutex;

int handleClient(int client_fd) {
    
    char buffer[1024];
    char nickname[1024];
    std::string finalMsg;
    int nickBytes = recv(client_fd, nickname, sizeof(nickname), 0);
    std::cout << nickname <<" Connected" << std::endl;
    int bytes;
    while(true) {
        memset(buffer, 0, sizeof(buffer));
        bytes = recv(client_fd, buffer, sizeof(buffer), 0);
        if (bytes <= 0) break;
        finalMsg = std::string(nickname) + ": " + std::string(buffer, bytes);
        std::cout << finalMsg << std::endl;

         {
        std::lock_guard<std::mutex> lock(clients_mutex);
        for(int client:clients) {
            if (client_fd != client) {
                send(client, finalMsg.c_str(), finalMsg.size(), 0);
            }
        }
        }

    }

    close(client_fd);
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(
            std::remove(clients.begin(), clients.end(), client_fd), 
            clients.end()
        );
    }
    return 0;
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: ./server <port>\n";
        return 1;
    }
    
    int port = std::stoi(argv[1]);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if(bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind");
        return 1;
    }

    listen(server_fd, 1);
    std::cout << "Server listening on port " << port << std::endl;

    while(true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if(client_fd < 0) continue;

        {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.push_back(client_fd);
        }
        std::thread t(handleClient, client_fd);
        t.detach();

        
    }
    
    close(server_fd);
    return 0;

}