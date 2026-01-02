# C++ Terminal TCP Chat

A simple terminal-based chat app in C++ allowing multiple clients to connect to a server and chat in real-time.

## Features
- Multiple clients
- Broadcast messages with nicknames
- Thread-safe handling of clients

## How to Run

### Compile & Run
```bash
g++ server/server.cpp -o server/server -pthread
g++ client/client.cpp -o client/client -pthread

./server/server <port>
./client/client <server_ip> <port> <nickname>
