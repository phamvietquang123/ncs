#include "client.h"

char mail_from[100];
char mail_to[100];  

char * Mail_Header(char *from,char *to,char *sub,char *content)
{
    time_t t;
    time(&t); 

    char *header = NULL;
    char date[26];
    char DATE_h[8 + strlen(date)];
    char sender[8 + strlen(from)];   //FROM: sender's_email\r\n
    char recep[6 + strlen(to)];      //TO: recepient's_email\r\n
    char subject[11 + strlen(sub)];
    char content_a[1 + strlen(content) + 2 + 1 + 1];
    strftime(date, (33), "%a %d %b %Y %H:%M:%S",localtime(&t));
    sprintf(DATE_h,"DATE: %s\r\n",date);
    sprintf(sender,"FROM: %s\r\n",from);
    sprintf(subject,"Subject: %s\r\n",sub);
    sprintf(recep,"TO: %s\r\n",to);

    sprintf(content_a,"%s\r\n",content);

    int header_length = strlen(DATE_h) + strlen(sender) + strlen(subject) + strlen(recep) + strlen(content_a);

    header = (char *) malloc(header_length * sizeof(char));

    memcpy(&header[0], &DATE_h, strlen(DATE_h));
    memcpy(&header[0 + strlen(DATE_h)], &sender , strlen(sender));
    memcpy(&header[0 + strlen(DATE_h) + strlen(sender)], &subject , strlen(subject));
    memcpy(&header[0 + strlen(DATE_h) + strlen(sender) + strlen(subject)], &recep , strlen(recep));
    memcpy(&header[0 + strlen(DATE_h) + strlen(sender) + strlen(subject) + strlen(recep)], &content_a , strlen(content_a));
    
    return header;
}




int main(int argc, char *argv[])
{
    int socket_id,n;
    int portno;
    struct sockaddr_in serv_addr;
    char cname[256];
    char buff[10240];
    bzero(cname,256);

    if(argc != 3)
    {
        perror("\nPlease enter the hostname and port number.\n");
        exit(-1);
    }
    portno = atoi(argv[2]);
    const char* IP = argv[1];
    socket_id = socket(AF_INET,SOCK_STREAM, 0);

    if(socket_id < 0)
    {
        perror("\nError occured while opening the socket!\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    
    //connect to the server
    int connect_id;
    connect_id = connect(socket_id,(struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(connect_id < 0)
    {
        perror("Error occured while connecting to server...\n");
        exit(-1);
    }
       
     int client_id;
    printf("Enter your ID: ");
    scanf("%d", &client_id);
    getchar(); 
    send(socket_id, (char*)&client_id, sizeof(client_id), 0);

    char server_message[10240];
    memset(server_message, 0, sizeof(server_message));
    recv(socket_id, server_message, sizeof(server_message), 0);
    printf("%s\n", server_message);

    while(1) {
        printf("Enter the command : ");
        bzero(cname, 256);
        fgets(cname,256,stdin);
        cname[strcspn(cname, "\n")] = 0;

        char code[4];       //to store the 3 digit response code received from server
        
        if(strcasecmp(cname,"HELO") == 0)
        {
            bzero(buff,10240);
            strcpy(buff,"HELO ");
            strcat(buff, domain);
            strcat(buff,"\r\n");
            n = send(socket_id,buff,strlen(buff),0);
            if(n < 0)
            {
                printf("\nError occured while writing to socket!\n");
            }
            printf("\nCLIENT : %s",buff);  //HELO domain
            bzero(buff,10240);
            n = recv(socket_id,buff,10239,0);
            if(n < 0)
            {
                printf("\nError occured while recving from socket!\n");
            }
            printf("SERVER : %s\n",buff);    //250 Hello domain

            code[0] = buff[0];
            code[1] = buff[1];
            code[2] = buff[2];
            code[3] = '\0';

            if(strcmp(code,"250") == 0)
            {
                printf("\nGo to next command...\n\n");
            }
            else{
                printf("\nError occured!\n\n");
            }
            fflush(stdin);
            continue;
        }
        else if(strcasecmp(cname,"MAIL FROM") == 0)
        {
            bzero(buff,10240);
            printf("\nEnter Sender Email id : ");
            scanf("%s",mail_from);
            strcpy(buff,"MAIL FROM:<");
            strcat(buff,mail_from);
            strcat(buff,">");
            strcat(buff,"\r\n");
            n = send(socket_id,buff,strlen(buff),0);
            if(n < 0)
            {
                printf("\nError occured while writing to socket!\n");
            }
            printf("\nCLIENT : %s",buff);  
            bzero(buff,10240);
            n = recv(socket_id,buff,10239,0);
            if(n < 0)
            {
                printf("\nError occured while recving from socket!\n");
            }
            printf("SERVER : %s\n",buff);  

           
            code[0] = buff[0];
            code[1] = buff[1];
            code[2] = buff[2];
            code[3] = '\0';

            if(strcmp(code,"250") == 0)
            {
                printf("\nGo to next command...\n\n");
            }
            else{
                printf("\nError occured!\n\n");
            }
            while (getchar() != '\n');
            continue;
        }
        else if(strcasecmp(cname,"RCPT TO") == 0)
        {
            bzero(buff,10240);
            printf("\nEnter Recipient Email id : ");
            scanf("%s",mail_to);
            strcpy(buff,"RCPT TO:<");
            strcat(buff,mail_to);
            strcat(buff,">");
            strcat(buff,"\r\n");
            n = send(socket_id,buff,strlen(buff),0);
            if(n < 0)
            {
                printf("\nError occured while writing to socket!\n");
            }
            printf("\nCLIENT : %s",buff);  
            bzero(buff,10240);
            n = recv(socket_id,buff,10239,0);
            if(n < 0)
            {
                printf("\nError occured while recving from socket!\n");
            }
            printf("SERVER : %s\n",buff);  
            //checking error
            code[0] = buff[0];
            code[1] = buff[1];
            code[2] = buff[2];
            code[3] = '\0';

            if(strcmp(code,"250") == 0)
            {
                printf("\nGo to next command...\n\n");
            }
            else{
                printf("\nError occured!\n\n");
            }
            while (getchar() != '\n');
            continue;
        }
        else if(strcasecmp(cname,"DATA") == 0)          
        {
            bzero(buff,10240);
            strcpy(buff,"DATA");
            strcat(buff,"\r\n");
            n = send(socket_id,buff,strlen(buff),0);
            if(n < 0)
            {
                printf("\nError occured while writing to socket!\n");
            }
            printf("\nCLIENT : %s",buff);  //DATA
            bzero(buff,10240);
            n = recv(socket_id,buff,10239,0);
            if(n < 0)
            {
                printf("\nError occured while recving from socket!\n");
            }
            printf("SERVER : %s\n",buff); 

            code[0] = buff[0];
            code[1] = buff[1];
            code[2] = buff[2];
            code[3] = '\0';

            if(strcmp(code,"354") == 0)
            {
                printf("\nrecvy to send header data!\n\n");
            }
            else{
                printf("\nError occured!\n\n");
            }

            char sub[150];
            char content[450];
            printf("\nEnter Subject : ");
            scanf("%[^\n]",sub);
            
            printf("\nEnter content : (Press Tab and Enter Key to end)\n");
            scanf("%[^\t]",content);

            bzero(buff,10240);
            //Mail_header function declared above
            strcpy(buff, Mail_Header(mail_from,mail_to,sub,content));  //assigning header to buffer
            
            n = send(socket_id,buff,strlen(buff),0);                     
            if(n < 0)
            {
                printf("\nError occured while writing to socket!\n");
            }
            printf("\nCLIENT : ====| Mail header & content |====\n%s\n",buff);  //header content
            
            bzero(buff,10240);
            strcpy(buff,".\r\n");
            n = send(socket_id,buff,strlen(buff),0);                    
            if(n < 0)
            {
                printf("\nError occured while writing to socket!\n");
            }
            
            bzero(buff,10240);
            n = recv(socket_id,buff,10239,0);  
            if(n < 0)
            {
                printf("\nError occured while recving from socket!\n");
            } 
            printf("SERVER : %s\n",buff);     //250 OK

            //checking error
            code[0] = buff[0];
            code[1] = buff[1];
            code[2] = buff[2];
            code[3] = '\0';

            if(strcmp(code,"250") == 0)
            {	
                printf("\nGo to next command...\n\n");
            }
            else{
                printf("\nError occured!\n\n");
            }
            while (getchar() != '\n');
            continue;                                  
        } 
        else if(strcasecmp(cname,"QUIT") == 0)
        {
            bzero(buff,10240);
            strcpy(buff,"QUIT");
            strcat(buff,"\r\n");
            n = send(socket_id,buff,strlen(buff),0);
            if(n < 0)
            {
                printf("\nError occured while writing to socket!\n");
            }
            printf("\nCLIENT : %s",buff);  //QUIT
            bzero(buff,10240);
            n = recv(socket_id,buff,10239,0);
            if(n < 0)
            {
                printf("\nError occured while recving from socket!\n");
            }
            printf("SERVER : %s\n",buff);    //221 Bye

            //checking error
            code[0] = buff[0];
            code[1] = buff[1];
            code[2] = buff[2];
            code[3] = '\0';

            if(strcmp(code,"221") == 0)
            {
                printf("\nConnection closed successfully with SMTP Server!\n\n");
            }
            else{
                printf("\nError occured!\n\n");
            }
            fflush(stdin);
            break;
        }
        else
        {	
        	printf("Invalid Command\n");
        	continue;
        }
    }
    
    close(socket_id);  
    return 0;
}