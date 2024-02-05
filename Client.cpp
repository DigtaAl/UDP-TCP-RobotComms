#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>

void receiveInt(int clientSocket) {
    while (true) {
        int receivedInt;
        int bytesRead = recv(clientSocket, &receivedInt, sizeof(receivedInt), 0);
        if (bytesRead <= 0) {
            std::cerr << "Server disconnected.\n";
            break;
        }

        std::cout << "Received : " << receivedInt << std::endl;
    }
}

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket\n";
        return -1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5555);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error connecting to server\n";
        close(clientSocket);
        return -1;
    }

    // Thread for receiving ints from the server
    std::thread receiveThread(receiveInt, clientSocket);

    // Main loop for sending ints to the server
    while (true) {
        int value;
        std::cout << "Number to server: \n";
        std::cin >> value;

        // Send the int to the server
        send(clientSocket, &value, sizeof(value), 0);
    }

    // Join the receive thread
    receiveThread.join();

    // Close the client socket
    close(clientSocket);

    return 0;
}
