#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define domain "server.smtp.com"

char * Mail_Header(char *from,char *to,char *sub,char *content);
