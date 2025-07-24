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
#define MAX_ATTEMPTS 5

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server, client;
    int client_len = sizeof(client);
    char buffer[BUFFER_SIZE];

    Sleep(1000); // wait to ensure time(NULL) changes
    srand((unsigned)time(NULL)); // seed the RNG
    int number_to_guess = rand() % 100 + 1;
    int attempts = 0;
    int game_over = 0;

    printf("Number to guess (for debug): %d\n", number_to_guess);

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket\n");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client, &client_len);

        if (game_over) {
            sendto(sock, "Game already ended. Restart the server to play again.", strlen("Game already ended. Restart the server to play again."), 0, (struct sockaddr*)&client, client_len);
            continue;
        }

        // Validate digits
        int isValid = 1;
        for (int i = 0; i < strlen(buffer); i++) {
            if (buffer[i] < '0' || buffer[i] > '9') {
                isValid = 0;
                break;
            }
        }

        if (!isValid) {
            sendto(sock, "Invalid input! Only numbers between 1-100 are allowed.", strlen("Invalid input! Only numbers between 1-100 are allowed."), 0, (struct sockaddr*)&client, client_len);
            continue;
        }

        int guess = atoi(buffer);

        if (guess < 1 || guess > 100) {
            sendto(sock, "Guess must be between 1 and 100!", strlen("Guess must be between 1 and 100!"), 0, (struct sockaddr*)&client, client_len);
            continue;
        }

        attempts++;

        printf("Received guess: %d (Attempt %d/%d)\n", guess, attempts, MAX_ATTEMPTS);

        // Correct guess
if (guess == number_to_guess) {
    sendto(sock, "Correct! You guessed it right!", strlen("Correct! You guessed it right!"), 0, (struct sockaddr*)&client, client_len);
    game_over = 1;
    continue;
}

// Check if attempts are over AFTER guess is wrong
if (attempts >= MAX_ATTEMPTS) {
    char msg[BUFFER_SIZE];
    snprintf(msg, BUFFER_SIZE, "You ran out of chances! Game over. The number was %d.", number_to_guess);
    sendto(sock, msg, strlen(msg), 0, (struct sockaddr*)&client, client_len);
    game_over = 1;
    continue;
}


if (MAX_ATTEMPTS - attempts == 1) {
    sendto(sock, "Only 1 chance left! Be careful.", strlen("Only 1 chance left! Be careful."), 0, (struct sockaddr*)&client, client_len);
    continue;  // Important: don't send hint in same round

        }

        // Hint
        if (guess < number_to_guess) {
            if (number_to_guess - guess <= 5) {
                sendto(sock, "Low, but close!", strlen("Low, but close!"), 0, (struct sockaddr*)&client, client_len);
            } else {
                sendto(sock, "Too low.", strlen("Too low."), 0, (struct sockaddr*)&client, client_len);
            }
        } else if (guess > number_to_guess) {
            if (guess - number_to_guess <= 5) {
                sendto(sock, "High, but close!", strlen("High, but close!"), 0, (struct sockaddr*)&client, client_len);
            } else {
                sendto(sock, "Too high.", strlen("Too high."), 0, (struct sockaddr*)&client, client_len);
            }
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
