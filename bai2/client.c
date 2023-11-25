
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define MESSAGE_LENGTH 1024

void receive_messages(SOCKET client_socket) {
    char message[MESSAGE_LENGTH];

    while (1) {
        memset(message, 0, sizeof(message));
        int bytes_received = recv(client_socket, message, sizeof(message), 0);
        if (bytes_received > 0) {
            printf("Received message: %s\n", message);
        }
        else if (bytes_received == 0) {
            printf("Server disconnected.\n");
            break;
        }
        else {
            printf("Error receiving message.\n");
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server;
    char message[MESSAGE_LENGTH];
    char server_ip[16];
    int server_port = 8888;

    printf("Enter Server IP: ");
    fgets(server_ip, sizeof(server_ip), stdin);
    server_ip[strlen(server_ip) - 1] = '\0';  

    
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d", WSAGetLastError());
        return 1;
    }

    
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d", WSAGetLastError());
        return 1;
    }

    
    server.sin_addr.s_addr = inet_addr(server_ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);

    
    if (connect(client_socket, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Connection error");
        return 1;
    }
    printf("Connected successfully to server.\n");

   
    int client_id;
    printf("Enter your ID: ");
    scanf("%d", &client_id);
    getchar();  
    send(client_socket, (char*)&client_id, sizeof(client_id), 0);

    
    char server_message[MESSAGE_LENGTH];
    memset(server_message, 0, sizeof(server_message));
    recv(client_socket, server_message, sizeof(server_message), 0);
    printf("%s\n", server_message);

    
    memset(server_message, 0, sizeof(server_message));
    int bytes_received = recv(client_socket, server_message, sizeof(server_message), 0);
    if (bytes_received > 0) {
        printf("List of users online:\n%s\n", server_message);
    }
    else {
        printf("Error receiving list of online users.\n");
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)receive_messages, (LPVOID)client_socket, 0, NULL);

    while (1) {
        printf("Enter message to send (or 'quit' to exit): ");
        fgets(message, sizeof(message), stdin);
        message[strlen(message) - 1] = '\0';  

        
        if (strcmp(message, "quit") == 0) {
            
            send(client_socket, message, strlen(message), 0);
            break;
        }

        
        send(client_socket, message, strlen(message), 0);
    }

    
    closesocket(client_socket);
    WSACleanup();
    return 0;
}