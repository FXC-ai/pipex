/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcoindre <fcoindre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 10:34:33 by fcoindre          #+#    #+#             */
/*   Updated: 2023/03/08 15:25:23 by fcoindre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>

char *cmd_exists(char *cmd, char *env[]);
void	ft_free_tabs(char **tab, int h);
int size_tab(char **tab);

void	ft_free_tabs(char **tab, int h)
{
	int	i;

	i = 0;
	while (i <= h)
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

int size_tab(char **tab)
{
    int count;

    count = 0;
    while (tab[count] != NULL)
        count++;
    return count;
}

char **create_tab_paths(char *env[])
{
    int     i;
    char    *paths;
    char    **paths_tab;

    i = 0;
    while (env[i])
    {
        if (strncmp(env[i], "PATH", 4) == 0)
        {
            paths = ft_substr(env[i], 5, ft_strlen(env[i])-4);
        }
        i++;
    }
    paths_tab = ft_split(paths, ':');
    free(paths);
    paths = NULL;
    return (paths_tab);
}

char *create_path_cmd(char *path, char *cmd)
{
    char *tmp;
    char *path_cmd;

    tmp = ft_strjoin(path, "/");
    path_cmd = ft_strjoin(tmp, cmd);
    free(tmp);
    tmp = NULL;

    return path_cmd;
}

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

    tab_cmd1 = ft_split(argv[2], ' ');
    path_cmd1 = cmd_exists(tab_cmd1[0], env);
    fd_infile = open(argv[1], O_RDONLY | O_CREAT, 0644);

	close(pipefd[0]);
 

    dup2(fd_infile,0);
    dup2(pipefd[1], 1); 
    close(fd_infile);
    // 0 => Entree standard
    // 1 => Sortie standard
    // 2 => Erreur standard
        
    if (execve(path_cmd1, tab_cmd1, env) == -1)
    {
        perror("child execve");
        close(pipefd[1]);
        exit(EXIT_FAILURE);
    }

    //printf("result_execve = %d\n", result_execve);
        
	close(pipefd[1]);
    
}

void    parent_process(int *pipefd, char *argv[], char *env[])
{
    int fd_outfile;
    char *path_cmd2;
    char **tab_cmd2;

    tab_cmd2 = ft_split(argv[3], ' ');
    path_cmd2 = cmd_exists(tab_cmd2[0], env);
    


    fd_outfile = open(argv[4], O_WRONLY | O_CREAT, 0644);

	close(pipefd[1]);
    dup2(pipefd[0],0);
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

int main(int argc, char *argv[], char *env[])
{
    pid_t pid;
    int wait_status;
    int pipefd[2];
    int pipe_status;

    if (argc != 5)
    {
        ft_putstr_fd("pipex : Invalid number of arguments\n", 2);

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
        child_process(pipefd, argv, env);
    }
    else
    {
        waitpid(-1, &wait_status, 0);
        parent_process(pipefd, argv, env);
    }
    return 0;
}

