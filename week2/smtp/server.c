#include "server.h"

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, n, portno, clilen;
    struct sockaddr_in serv_addr, cli_addr;
    char buff[10240];    

    sockfd = socket(AF_INET, SOCK_STREAM,0);
    if(sockfd < 0)
    {
        perror("\nError occured while creating the socket!\n");
        exit(-1);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
	{
		perror("\nSocket binding failed!\n");
        exit(-1);
	}

    if(listen(sockfd,5) < 0) {
        perror("\nUnable to listen to the port");
        exit(-1);
    };

    while(1) {
    printf("Awaiting Connection...\n");
    int clilen = sizeof(cli_addr);
    int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) 
    {
    	perror("\nError occured while accepting on socket!\n");
        exit(-1);
    }
    printf("Connection received\n");
    //Implementing responses which SMTP Server sends to Client for Acknowledgement in the process of Email Receiving
        while(1) {
        bzero(buff,10240);
        n = recv(newsockfd,buff,10239,0);
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
        printf("Connection close");
    }
    close(sockfd);
    return 0;
}