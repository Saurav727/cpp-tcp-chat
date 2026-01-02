# C++ Terminal TCP Chat

A simple terminal-based chat app in C++ allowing multiple clients to connect to a server and chat in real-time.

## Features
- Multiple clients
- Broadcast messages with nicknames
- Thread-safe handling of clients

## How to Run

### Compile
```bash
g++ server/server.cpp -o server/server -pthread
g++ client/client.cpp -o client/client -pthread
