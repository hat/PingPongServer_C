#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
 
int main(int argc , char *argv[])
{
    int sock;
    int amt_read;
    struct sockaddr_in server;
    char message[1000]; 
    char server_reply[2000];
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");  
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( atoi(argv[1]) );
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }  
    puts("Connected\n"); 
    //keep communicating with server
    while(1)
    {
        bzero(&server_reply, 2000);
        bzero(&message, 1000);
        if ((amt_read = read(sock, server_reply, 2000)) > 0)
        {
            write(1, server_reply, 2000);
        }
        if ((amt_read = read(1, message, 1000)) > 0)
        {
            write(sock, message, 1000);
        }
    }   
    close(sock);
    return 0;
}