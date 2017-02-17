/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thendric <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/17 11:50:45 by thendric          #+#    #+#             */
/*   Updated: 2017/02/17 12:52:36 by thendric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server.h>

void	error(const char *msg)
{
    perror(msg);
    exit(1);
}

void	listen_server(t_server *serv_env)
{
    pid_t pid;

	listen(serv_env->sockfd, 5);
	serv_env->clilen = sizeof(serv_env->cli_addr);
	serv_env->newsockfd = accept(serv_env->sockfd,
		(struct sockaddr *) &serv_env->cli_addr, &serv_env->clilen);
	if (serv_env->newsockfd < 0)
		error("ERROR on accept");
	if (serv_env->newsockfd > 0)
	{
		write(serv_env->newsockfd,"Pong\n",5);
		write(serv_env->newsockfd,"Pong\n",5);
		pid = fork();
		if (pid == 0)
		{
			while ((serv_env->read = read(serv_env->newsockfd,
					serv_env->buffer, BUF_SIZE)) > 0)
				write(1, serv_env->buffer, serv_env->read);
		}
		else
		{
			while ((serv_env->read = read(1, serv_env->buffer, BUF_SIZE)) > 0)
				write(serv_env->newsockfd, serv_env->buffer, serv_env->read);
		}
	}
}

void    start_server(char *argv[], t_server *serv_env)
{
    printf("Server starting...\n");
    serv_env->sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_env->sockfd < 0)
        error("ERROR opening socket");
    serv_env->portno = atoi(argv[1]);
    serv_env->serv_addr.sin_family = PF_INET;
    serv_env->serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_env->serv_addr.sin_port = htons(serv_env->portno);
    if (bind(serv_env->sockfd, (struct sockaddr *) &serv_env->serv_addr, sizeof(serv_env->serv_addr)) < 0)
        error("ERROR on binding");
    while (1)
        listen_server(serv_env);
}

void    server_daemon(char *argv[], t_server *serv_env)
{
    int i;
    pid_t process_id;
    pid_t sid;

    process_id = 0;
    sid = 0;
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
    umask(0);
    sid = setsid();
    if (sid < 0)
    {
        exit (1);
    }
    chdir("/tmp");
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    sleep(1);
    start_server(argv, serv_env);
}

int     main(int argc, char *argv[])
{
    t_server    *serv_env;

    serv_env = (t_server *)malloc(sizeof(t_server));
    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    if (strcmp(argv[1], "-D") == 0)
    {
        server_daemon(argv, serv_env);
    }
    start_server(argv, serv_env);
    close(serv_env->newsockfd);
    close(serv_env->sockfd);
    return (0);
}
