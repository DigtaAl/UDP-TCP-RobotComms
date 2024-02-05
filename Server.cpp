#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>
#include <algorithm>

std::vector<int> clients;
bool zeroSent = false;  // Menandai apakah nilai 0 telah dikirim ke client lainnya

void broadcastInt(int value, int senderSocket) {
    for (auto& client : clients) {
        //if (client != senderSocket) {
            send(client, &value, sizeof(value), 0);
        //}
    }
}

void handleClient(int clientSocket, sockaddr_in clientAddr) {
    char clientIp[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, sizeof(clientIp));
    std::cout << "Client baru " << clientIp << ".\n";

    while (true) {
        int receivedInt;
        int bytesRead = recv(clientSocket, &receivedInt, sizeof(receivedInt), 0);
        if (bytesRead <= 0) {
            std::cout << clientIp << " disconnected.\n";
            broadcastInt(0, clientSocket);
            break;
        }
        std::cout << "[" << clientIp << "] Received : " << receivedInt << std::endl;

        // Jika nilai yang diterima adalah 0 dan belum dikirim sebelumnya, kirim ke semua client
        if (receivedInt == 0 && !zeroSent) {
            zeroSent = true;
            broadcastInt(receivedInt, clientSocket);
        } else if (receivedInt != 0) {
            zeroSent = false;
            // Broadcast the received int to all clients except the sender
            broadcastInt(receivedInt, clientSocket);
        }
    }

    // Remove the disconnected client from the vector
    clients.erase(std::remove(clients.begin(), clients.end(), clientSocket), clients.end());

    // Close the client socket
    close(clientSocket);
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket\n";
        return -1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5555);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket\n";
        close(serverSocket);
        return -1;
    }

    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening on socket\n";
        close(serverSocket);
        return -1;
    }

    std::cout << "Server is listening on port 5555\n";

    while (true) {
        sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);

        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == -1) {
            std::cerr << "Error accepting connection\n";
            continue;
        }

        // Add the client to the vector
        clients.push_back(clientSocket);

        // Send a welcome message to the new client
        int welcomeMessage = 0;
        send(clientSocket, &welcomeMessage, sizeof(welcomeMessage), 0);

        // Handle communication with the client in a separate thread
        std::thread clientThread(handleClient, clientSocket, clientAddr);
        clientThread.detach();
    }

    // Close the server socket
    close(serverSocket);

    return 0;
}
