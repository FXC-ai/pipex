/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcoindre <fcoindre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 10:34:33 by fcoindre          #+#    #+#             */
/*   Updated: 2023/03/10 15:01:00 by fcoindre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int main(int argc, char *argv[], char *env[])
{
    pid_t pid;
    int wait_status;
    int pipefd[2];
    int pipe_status;

    if (argc != 5)
    {
        ft_putstr_fd("pipex : Invalid number of arguments\n", 2);
        //exit (128);
        return (1);
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
        child_process(pipefd, argv, env);
    }
    else
    {
        waitpid(-1, &wait_status, 0);
        parent_process(pipefd, argv, env);
    }
    return 0;
}

