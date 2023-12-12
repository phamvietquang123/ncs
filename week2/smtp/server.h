#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/select.h>
#include <pthread.h>

#define domain "server.smtp.com"

#ifndef SERVER_H
#define SERVER_H

void *handle_connection(void *arg);

#endif