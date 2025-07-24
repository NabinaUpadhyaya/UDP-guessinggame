// udp_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char buffer[BUFFER_SIZE], response[BUFFER_SIZE];
    int server_len = sizeof(server);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    // Create socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket\n");
        return 1;
    }

    // Setup server
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(SERVER_IP);

    printf("Welcome! You have 5 chances to guess a number between 1 and 100:\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        memset(response, 0, BUFFER_SIZE);

        printf("Your guess: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // remove newline

        sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&server, server_len);
        recvfrom(sock, response, BUFFER_SIZE, 0, NULL, NULL);

        printf("Server says: %s\n", response);

        // ✅ This handles: "Correct! You guessed it right!"
        if (strncmp(response, "Correct!", 8) == 0 ||
            strncmp(response, "You ran out", 11) == 0 ||
            strncmp(response, "Game already", 12) == 0) {
            break;
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
