
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") 

#define MAX_CLIENTS 10
#define MESSAGE_LENGTH 1024

struct client_info {
    int id;
    SOCKET socket;
    struct sockaddr_in addr;
};

struct client_info clients[MAX_CLIENTS];
int num_clients = 0;
CRITICAL_SECTION cs;

void print_clients() {
    printf("List of users online:\n");
    for (int i = 0; i < num_clients; i++) {
        printf("%d\n", clients[i].id);
    }
    printf("\n");
}

int find_client_by_id(int id) {
    for (int i = 0; i < num_clients; i++) {
        if (clients[i].id == id) {
            return i;
        }
    }
    return -1;
}

void broadcast_message(char *message, int sender_id) {
    for (int i = 0; i < num_clients; i++) {
        if (clients[i].id != sender_id) {
            send(clients[i].socket, message, strlen(message), 0);
        }
    }
}

DWORD WINAPI handle_client(LPVOID lpParam) {
    int client_index = (int)lpParam;
    char buffer[MESSAGE_LENGTH];
    char message[MESSAGE_LENGTH + 50];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int recv_size = recv(clients[client_index].socket, buffer, MESSAGE_LENGTH, 0);
        if (recv_size == SOCKET_ERROR || recv_size == 0) {
            printf("Client %d disconnected\n", clients[client_index].id);
            closesocket(clients[client_index].socket);

            EnterCriticalSection(&cs);
            num_clients--;
            for (int i = client_index; i < num_clients; i++) {
                clients[i] = clients[i + 1];
            }
            LeaveCriticalSection(&cs);

            print_clients();
            char online_users[1024];
            memset(online_users, 0, sizeof(online_users));
            for (int i = 0; i < num_clients; i++) {
                char user_info[50];
                sprintf(user_info, "%d\n", clients[i].id);
                strcat(online_users, user_info);
            }
            broadcast_message(online_users, -1); 

            break;
        } else {
            sprintf(message, "[%d] %s", clients[client_index].id, buffer);
            broadcast_message(message, clients[client_index].id);
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    WSADATA wsa;
    SOCKET server_socket, new_socket;
    struct sockaddr_in server, client;
    int c;

    
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d", WSAGetLastError());
        return 1;
    }
    printf("");

        
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        return 1;
    }
    printf("");

    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code: %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("");

    
    listen(server_socket, 3);

   
    InitializeCriticalSection(&cs);

   
    printf("Waiting for clients to connect...\n");
    c = sizeof(struct sockaddr_in);

    while ((new_socket = accept(server_socket, (struct sockaddr *)&client, &c)) != INVALID_SOCKET) {
        printf("Connection accepted.\n");

        int client_id;
        recv(new_socket, (char *)&client_id, sizeof(client_id), 0);


       
        EnterCriticalSection(&cs);
        clients[num_clients].id = client_id;
        clients[num_clients].socket = new_socket;
        clients[num_clients].addr = client;
        num_clients++;
        LeaveCriticalSection(&cs);

        printf("[%d] connected. Let's chat.\n", client_id);

    
        send(new_socket, "Connected to the server. Let's chat.", strlen("Connected to the server. Let's chat."), 0);

        
        print_clients();
        char online_users[1024];
        memset(online_users, 0, sizeof(online_users));
        EnterCriticalSection(&cs);
        for (int i = 0; i < num_clients; i++) {
            char user_info[50];
            sprintf(user_info, "%d\n", clients[i].id);
            strcat(online_users, user_info);
        }
        LeaveCriticalSection(&cs);
        broadcast_message(online_users, -1); 

        
        DWORD thread_id;
        HANDLE thread_handle = CreateThread(NULL, 0, handle_client, (LPVOID)(num_clients - 1), 0, &thread_id);
        if (thread_handle == NULL) {
            printf("Failed to create thread for client %d\n", client_id);
        } else {
            CloseHandle(thread_handle);
        }
    }

    if (new_socket == INVALID_SOCKET) {
        printf("Accept failed with error code: %d", WSAGetLastError());
        return 1;
    }

    closesocket(server_socket);
    WSACleanup();

    DeleteCriticalSection(&cs);

    return 0;
}