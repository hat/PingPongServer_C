#ifndef SERVER_H
# define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

typedef struct s_server
{
	int sockfd;
    int newsockfd;
    int portno;
    socklen_t clilen;
    long read;
    char buffer[2000];
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    int n;
    socklen_t   clilen;
}			t_server;

#endif