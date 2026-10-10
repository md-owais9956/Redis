
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>

#include <csignal>
#include <iostream>
#include <string>

namespace {
volatile std::sig_atomic_t running = 1;

constexpr unsigned short PORT = 6379;
constexpr int BUFFER_SIZE = 1024;

void handleSignal(int) {
    running = 0;
}

void handleClient(SOCKET clientSocket) {
    char buffer[BUFFER_SIZE];

    std::cout << "Client connected.\n";

    while (running) {
        // Use select so the server can notice shutdown requests.
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(clientSocket, &readSet);

        timeval timeout{};
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int ready = select(
            0, &readSet, nullptr, nullptr, &timeout
        );

        if (ready == 0) {
            continue; // Timeout: check running again.
        }

        if (ready == SOCKET_ERROR) {
            std::cerr << "Select failed: "
                      << WSAGetLastError() << '\n';
            break;
        }

        int bytesReceived = recv(
            clientSocket, buffer, BUFFER_SIZE, 0
        );

        if (bytesReceived == 0) {
            std::cout << "Client disconnected.\n";
            break;
        }

        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Receive failed: "
                      << WSAGetLastError() << '\n';
            break;
        }

        std::string message(buffer, bytesReceived);
        std::cout << "Received: " << message;
        std::cout.flush();

        // Echo all received bytes, including partial sends.
        int totalSent = 0;

        while (totalSent < bytesReceived && running) {
            int bytesSent = send(
                clientSocket,
                buffer + totalSent,
                bytesReceived - totalSent,
                0
            );

            if (bytesSent == SOCKET_ERROR || bytesSent == 0) {
                std::cerr << "Send failed: "
                          << WSAGetLastError() << '\n';
                closesocket(clientSocket);
                return;
            }

            totalSent += bytesSent;
        }
    }

    closesocket(clientSocket);
    std::cout << "Client connection closed.\n";
}
} // namespace

int main() {
    std::signal(SIGINT, handleSignal);
    std::signal(SIGTERM, handleSignal);

    // Initialize Windows networking.
    WSADATA wsaData{};

    int startupResult = WSAStartup(
        MAKEWORD(2, 2), &wsaData
    );

    if (startupResult != 0) {
        std::cerr << "WSAStartup failed: "
                  << startupResult << '\n';
        return 1;
    }

    SOCKET serverSocket = socket(
        AF_INET, SOCK_STREAM, IPPROTO_TCP
    );

    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: "
                  << WSAGetLastError() << '\n';
        WSACleanup();
        return 1;
    }

    BOOL reuseAddress = TRUE;

    if (setsockopt(
            serverSocket,
            SOL_SOCKET,
            SO_REUSEADDR,
            reinterpret_cast<const char*>(&reuseAddress),
            sizeof(reuseAddress)
        ) == SOCKET_ERROR) {
        std::cerr << "setsockopt failed: "
                  << WSAGetLastError() << '\n';
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(PORT);

    if (bind(
            serverSocket,
            reinterpret_cast<sockaddr*>(&serverAddress),
            sizeof(serverAddress)
        ) == SOCKET_ERROR) {
        std::cerr << "Bind failed: "
                  << WSAGetLastError() << '\n';
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed: "
                  << WSAGetLastError() << '\n';
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Redis From Scratch - TCP Server\n";
    std::cout << "Listening on port " << PORT << "...\n";

    while (running) {
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(serverSocket, &readSet);

        timeval timeout{};
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int ready = select(
            0, &readSet, nullptr, nullptr, &timeout
        );

        if (ready == 0) {
            continue;
        }

        if (ready == SOCKET_ERROR) {
            std::cerr << "Server select failed: "
                      << WSAGetLastError() << '\n';
            break;
        }

        sockaddr_in clientAddress{};
        int clientLength = sizeof(clientAddress);

        SOCKET clientSocket = accept(
            serverSocket,
            reinterpret_cast<sockaddr*>(&clientAddress),
            &clientLength
        );

        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed: "
                      << WSAGetLastError() << '\n';
            continue;
        }

        handleClient(clientSocket);
    }

    closesocket(serverSocket);
    WSACleanup();

    std::cout << "\nTCP server shut down.\n";
    return 0;
}
