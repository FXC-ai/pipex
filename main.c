/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcoindre <fcoindre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 10:34:33 by fcoindre          #+#    #+#             */
/*   Updated: 2023/03/03 14:57:47 by fcoindre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>



int main(int argc, char *argv[], char *env[])
{
    (void) argv;
    (void) env;
    pid_t pid;
    int wait_status;
    
    int pipefd[2];
    int pipe_status;
    char buf[1024];

    int result_execve;


    if (argc != 5)
    {
        ft_printf("Invalid number of arguments");
        return (0);
    }
    
    pipe_status = pipe(pipefd);
    if (pipe_status == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid == 0)
    {
        char *arguments[3];
        arguments[0] = "/bin/ls";
        arguments[1] = "-l";
        arguments[2] = NULL;
		close(pipefd[0]);

        // Ici il fau


        dup2(pipefd[1], 1);
        result_execve = execve(arguments[0], arguments, env);

        
		//printf("Check write : %zd\n", write(pipefd[1], "test", 5));
		close(pipefd[1]);
        return 0;
    }
    else
    {
        wait(&wait_status);

		close(pipefd[1]);
		printf("Check read : %zd\n", read(pipefd[0], buf, 15));
		printf("Message recu du processus fils : %s", buf);
		close(pipefd[0]);
    }

    


    return 0;
}

