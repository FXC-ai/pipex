/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcoindre <fcoindre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:50:50 by fcoindre          #+#    #+#             */
/*   Updated: 2023/03/08 16:41:08 by fcoindre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char *cmd_exists(char *cmd, char *env[])
{
    int i;
    char **tab_paths;    
    char *path_to_test;

    tab_paths = create_tab_paths(env);

    i = 0;
    while (tab_paths[i])
    {
        path_to_test = create_path_cmd(tab_paths[i], cmd);
        if (access(path_to_test, F_OK) == 0)
        {
            ft_free_tabs(tab_paths, size_tab(tab_paths));
            return path_to_test;
        }
        free(path_to_test);
        path_to_test = NULL;
        i++;
    }
    ft_free_tabs(tab_paths, size_tab(tab_paths));
    return (NULL);
}

void    child_process(int *pipefd, char *argv[], char *env[])
{

    int fd_infile;
    char *path_cmd1;
    char **tab_cmd1;

	close(pipefd[0]);

    tab_cmd1 = ft_split(argv[2], ' ');
    path_cmd1 = cmd_exists(tab_cmd1[0], env);



    if (path_cmd1 == NULL)
    {
        free(tab_cmd1);
        close(pipefd[1]);
        exit(EXIT_FAILURE);
    }
    
    fd_infile = open(argv[1], O_RDONLY | O_CREAT, 0644);

    if (fd_infile == -1)
    {
        free(tab_cmd1);
        close(pipefd[1]);
        perror("child open");
        exit(EXIT_FAILURE);
    }
    
    if(dup2(fd_infile,0) == -1 || dup2(pipefd[1], 1) == -1)
    {
        free(tab_cmd1);
        close(fd_infile);
        close(pipefd[1]);
        perror("child dup2");
        exit(EXIT_FAILURE);
    }

    close(fd_infile);
    
    if (execve(path_cmd1, tab_cmd1, env) == -1)
    {
        perror("child execve");
        close(pipefd[1]);
        exit(EXIT_FAILURE);
    }
    
	close(pipefd[1]);
}

void    parent_process(int *pipefd, char *argv[], char *env[])
{
    int fd_outfile;
    char *path_cmd2;
    char **tab_cmd2;

	close(pipefd[1]);
    
    tab_cmd2 = ft_split(argv[3], ' ');
    path_cmd2 = cmd_exists(tab_cmd2[0], env);

    if (path_cmd2 == NULL)
    {
        free(tab_cmd2);
        close(pipefd[0]);
        exit(EXIT_FAILURE);
    }

    fd_outfile = open(argv[4], O_WRONLY | O_CREAT, 0644);

    if (fd_outfile == -1)
    {
        free(tab_cmd2);
        close(pipefd[0]);
        perror("child open");
        exit(EXIT_FAILURE);
    }

    if (dup2(pipefd[0],0) == -1)
    {
        perror("dup2 parent");
        close(pipefd[1]);
        exit(EXIT_FAILURE);
    }



    dup2(fd_outfile,1);
    close(fd_outfile);
    
    if (execve(path_cmd2, tab_cmd2, env) == -1)
    {
        perror("parent execve");
        close(pipefd[1]);
        exit(EXIT_FAILURE);
    }
    
    close(pipefd[0]);
}