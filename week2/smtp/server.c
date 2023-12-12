#include "server.h"

void *handle_connection(void *arg) {
    int newsockfd = *((int *)arg);
    char buff[10240];

    while (1) {
        bzero(buff,10240);
       int n = recv(newsockfd,buff,10239,0);
        if (n < 0)
        {
            printf("\nError occured while reading from socket!\n");
            break;
        }
        else
        {
            buff[n] = '\0';
            if(strstr(buff,"HELO")!=NULL)
            {
                printf("RECEIVED : %s",buff);
                bzero(buff,10240);
                strcpy(buff,"250 Hello ");
                strcat(buff, domain);
                printf("SENT : %s\n\n",buff);
                n = send(newsockfd,buff,strlen(buff),0);
                if (n<0) {  perror("Error occured while writing to socket!"); }
            }
            else if(strstr(buff,"MAIL FROM")!=NULL)
            {
                printf("RECEIVED : %s",buff);
                bzero(buff,10240);
                strcpy(buff,"250 OK");
                printf("SENT : %s\n\n",buff);
                n = send(newsockfd,buff,strlen(buff),0);
                if (n<0) {  perror("Error occured while writing to socket!"); }
            }
            else if(strstr(buff,"RCPT TO")!=NULL)
            {
                printf("RECEIVED : %s",buff);
                bzero(buff,10240);
                strcpy(buff,"250 OK");
                printf("SENT : %s\n\n",buff);
                n = send(newsockfd,buff,strlen(buff),0);
                if (n<0) {  perror("Error occured while writing to socket!"); }
            }
            else if(strstr(buff,"DATA")!=NULL)
            {
                printf("RECEIVED : %s",buff);
                bzero(buff,10240);
                strcpy(buff,"354 Send message content; end with <CRLF>.<CRLF>");
                printf("SENT : %s\n\n",buff);
                n = send(newsockfd,buff,strlen(buff),0);
                if (n < 0) {  perror("Error occured while writing to socket!"); }

                bzero(buff,10240);
                n = read(newsockfd,buff,10239);
                if (n < 0){ printf("\nError occured while reading from socket!\n"); break; }
                printf("\n\n----------| Received Email Header & Content |----------\n\n%s\n",buff);
                printf("-------------------------------------------------------\n\n");
                                        
                bzero(buff,10240);
                n = read(newsockfd,buff,10239);
                if (n < 0){ printf("\nError occured while reading from socket!\n"); break; }
            
                if(strstr(buff,".")!=NULL){
                    printf("RECEIVED : %s",buff);
                    bzero(buff,10240);
                  
                    strcpy(buff,"250 OK, message accepted for delivery.");
                    printf("SENT : %s\n\n",buff);
                    n = send(newsockfd,buff,strlen(buff),0);
                    if (n < 0) {  perror("Error occured while writing to socket!"); }
                }
            }
            else if(strstr(buff,"QUIT") != NULL)
            {
                printf("RECEIVED : %s",buff);
                bzero(buff,10240);
                strcpy(buff,"221 Bye");
                printf("SENT : %s\n\n",buff);
                n = send(newsockfd,buff,strlen(buff),0);
                if (n<0) {  perror("Error occured while writing to socket!"); }
                break;
            }   
        }
    }

    close(newsockfd);
    printf("Connection closed\n");
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    struct sockaddr_in serv_addr, cli_addr;
    pthread_t thread_id;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 5) < 0) {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Awaiting Connection...\n");
        socklen_t clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

        if (newsockfd < 0) {
            perror("Error accepting on socket");
            exit(EXIT_FAILURE);
        }

        printf("Connection received\n");
        int client_id;
        recv(newsockfd, (char *)&client_id, sizeof(client_id), 0);

        printf("[%d] connected. Let's chat.\n", client_id);
        send(newsockfd, "Connected to the server.", strlen("Connected to the server"), 0);

        if (pthread_create(&thread_id, NULL, handle_connection, (void *)&newsockfd) != 0) {
            perror("Error creating thread");
            close(newsockfd);
            continue;  // Continue with the next iteration
        }
    }

    close(sockfd);
    return 0;
}
