// udp_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h> 
#include <windows.h>  

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
   WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server, client;
    int client_len = sizeof(client);
    char buffer[BUFFER_SIZE];

    Sleep(1000);                  // wait to ensure time(NULL) changes
    srand(time(NULL));            // seed the RNG
    int number_to_guess = rand() % 100 + 1;

    printf("Number to guess (for debug): %d\n", number_to_guess);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    // Create UDP socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket\n");
        return 1;
    }

    // Setup server address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind
    if (bind(sock, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        int guess;
        memset(buffer, 0, BUFFER_SIZE);
        recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client, &client_len);
        guess = atoi(buffer);

        printf("Received guess: %d\n", guess);

        if (guess < number_to_guess) {
    if (number_to_guess - guess <= 5) {
        sendto(sock, "low, but close!", strlen("low, but close!"), 0, (struct sockaddr*)&client, client_len);
    } else {
        sendto(sock, "Too low", strlen("Too low"), 0, (struct sockaddr*)&client, client_len);
    }
} else if (guess > number_to_guess) {
    if (guess - number_to_guess <= 5) {
        sendto(sock, "high, but close!", strlen(" high, but close!"), 0, (struct sockaddr*)&client, client_len);
    } else {
        sendto(sock, "Too high", strlen("Too high"), 0, (struct sockaddr*)&client, client_len);
    }
} else {
    sendto(sock, "Correct!", strlen("Correct!"), 0, (struct sockaddr*)&client, client_len);
    break;
}

    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
