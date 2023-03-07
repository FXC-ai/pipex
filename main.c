/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcoindre <fcoindre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 10:34:33 by fcoindre          #+#    #+#             */
/*   Updated: 2023/03/07 21:10:02 by fcoindre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>

int cmd_exists(char *cmd, char *env[]);
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

int cmd_exists(char *cmd, char *env[])
{
    int i;
    char **paths_cmd;
    char *paths;
    char *path_to_test;
    char *tmp;

    i = 0;
    while (env[i])
    {
        if (strncmp(env[i], "PATH", 4) == 0)
        {
            paths = ft_substr(env[i], 5, ft_strlen(env[i])-4);
        }
        i++;
    }

    
    paths_cmd = ft_split(paths, ':');
    free(paths);
    paths = NULL;


    i = 0;
    while (paths_cmd[i])
    {
        // A mettre dans une fontion externe char *create_path_cmd(char *cmd, char *env[]) ?
        tmp = ft_strjoin(paths_cmd[i], "/");
        path_to_test = ft_strjoin(tmp, cmd);
        free(tmp);
        tmp = NULL;
        if (access(path_to_test, F_OK) == 0)
        {
            printf("[%d] path %s : OK\n", i, path_to_test);
            free(path_to_test);
            path_to_test = NULL;
            break;
        }
        free(path_to_test);
        path_to_test = NULL;
        i++;
    }
    
    ft_free_tabs(paths_cmd, size_tab(paths_cmd));

    return 4200000;

}




int main(int argc, char *argv[], char *env[])
{


    cmd_exists("awk", env);
    (void) argc;
    (void) argv;

    
    /*
    (void) env;
    pid_t pid;
    int wait_status;
    char *arguments[4];

    int fd_infile;
    int fd_outfile;

    int access_check;


    int pipefd[2];
    int pipe_status;


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
        
        arguments[0] = "/bin/cat";
        arguments[1] = "-e";
        arguments[2] = NULL;
        arguments[3] = NULL;

		close(pipefd[0]);

        // Ici il faut open le infile et mettre dans le outfile le resultat

        fd_infile = open("infile", O_RDONLY | O_CREAT, 0644);

        printf("fd_infile = %d\n", fd_infile);
        // => pour ce processus le 1 ne redirige plus vers la sortie standard mais vers le pipe
        
        dup2(fd_infile,0);
        dup2(pipefd[1], 1); 
        // 0 => Entree standard
        // 1 => Sortie standard
        // 2 => Erreur standard
        
        
        //on remplace le processus par un autre
        result_execve = execve(arguments[0], arguments, env);

        
		// printf("Check write : %zd\n", write(pipefd[1], "test", 5));
		close(pipefd[1]);
        return 0;
    }
    else
    {
        waitpid(-1, &wait_status, 0);

        arguments[0] = "/usr/bin/wc";
        arguments[1] = "-w";
        arguments[2] = NULL;
        arguments[3] = NULL;

		close(pipefd[1]);

        fd_outfile = open("outfile", O_WRONLY | O_CREAT,0644);


        dup2(pipefd[0],0);
        dup2(fd_outfile,1);
        

        access_check = access("/usr/bin/wc", F_OK);
        //printf("access_check = %d\n", access_check);
        
		//printf("Check read : %d\n", check_read = read(pipefd[0], buf, 1023));
        //buf[check_read] = '\0';
		//printf("Contenu du pipefd[0] : \n%s", buf);
        
        


        result_execve = execve(arguments[0], arguments, env);
        
        //printf("result_execve = %d", result_execve);
        
		close(pipefd[0]);
    }

    
    */

    return 0;
}

