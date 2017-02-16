/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <string.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void    start_server(char *argv[])
{
    int sockfd;
    int newsockfd;
    int portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    int n;

    printf("Server starting...\n");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) 
          error("ERROR on accept");
    if (newsockfd > 0)
    {
        // printf("Pong %d\n",newsockfd);
        // printf("Port: %d\n", cli_addr.sin_port);
        // printf("Port: %d\n", cli_addr.sin_addr.s_addr);
        n = write(newsockfd,"Pong\n",5);
        //if (n < 0)
            //error("ERROR writing to socket");
        n = write(newsockfd,"Pong\n",5);
        if (n < 0)
            error("ERROR writing to socket");
    }
}

int main(int argc, char *argv[])
{
    int sockfd;
    int newsockfd;
    int portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    int n;

    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    //Check for -D to run as Daemon
    int i;
    pid_t process_id;
    pid_t sid;
    FILE *fp;

    process_id = 0;
    sid = 0;
    fp = NULL;
    if (strcmp(argv[1], "-D") == 0)
    {
        printf("Run as Daemon\n");
        process_id = fork();
        if (process_id < 0)
        {
            printf("fork failed\n");
            exit(1);
        }
        if (process_id > 0)
        {
            printf("process_id of child process %d \n", process_id);
            exit(0);
        }
        // chdir("/");
        // close(STDIN_FILENO);
        // close(STDOUT_FILENO);
        // close(STDERR_FILENO);
        // fp = fopen ("Log.txt", "w+");
        while (1)
        {
            start_server(argv);
            sleep(1);
        }
    }
    start_server(argv);


    // sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // if (sockfd < 0) 
    //     error("ERROR opening socket");
    // bzero((char *) &serv_addr, sizeof(serv_addr));
    // portno = atoi(argv[1]);
    // serv_addr.sin_family = AF_INET;
    // serv_addr.sin_addr.s_addr = INADDR_ANY;
    // serv_addr.sin_port = htons(portno);
    // if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
    //     error("ERROR on binding");
    // listen(sockfd,5);
    // clilen = sizeof(cli_addr);
    // newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    // if (newsockfd < 0) 
    //       error("ERROR on accept");
    // if (newsockfd > 0)
    // {
    //     n = write(newsockfd,"Pong\n",5);
    //     if (n < 0)
    //         error("ERROR writing to socket");
    //     n = write(newsockfd,"Pong\n",5);
    //     if (n < 0)
    //         error("ERROR writing to socket");
    // }

    //ALLOWS CLIENT TO SEND A MESSAGE TO THE SERVER
    //bzero(buffer,256);
    //n = read(newsockfd,buffer,255);
    //if (n < 0) error("ERROR reading from socket");
    //printf("Here is the message: %s\n",buffer);
    //n = write(newsockfd,"I got your message",18);

    close(newsockfd);
    close(sockfd);
    return 0; 
}