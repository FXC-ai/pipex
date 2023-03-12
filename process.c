/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcoindre <fcoindre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:50:50 by fcoindre          #+#    #+#             */
/*   Updated: 2023/03/10 18:43:21 by fcoindre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//cmd_exists free tout si la commande existe pas

char	**create_tab_paths(char *env[])
{
	int		i;
	char	*paths;
	char	**paths_tab;

	i = 0;
	while (env[i])
	{
		if (strncmp(env[i], "PATH", 4) == 0)
			paths = ft_substr(env[i], 5, ft_strlen(env[i]) - 4);
		i++;
	}
	paths_tab = ft_split(paths, ':');
	free(paths);
	paths = NULL;
	return (paths_tab);
}

char	*create_path_cmd(char *path, char *cmd)
{
	char	*tmp;
	char	*path_cmd;

	tmp = ft_strjoin(path, "/");
	path_cmd = ft_strjoin(tmp, cmd);
	free(tmp);
	tmp = NULL;
	return (path_cmd);
}

char	*cmd_exists(char *cmd, char *env[])
{
	int		i;
	char	**tab_paths;
	char	*path_to_test;

	tab_paths = create_tab_paths(env);
	i = 0;
	while (tab_paths[i])
	{
		path_to_test = create_path_cmd(tab_paths[i], cmd);
		if (access(path_to_test, F_OK) == 0)
		{
			ft_free_tabs(tab_paths, size_tab(tab_paths));
			return (path_to_test);
		}
		free(path_to_test);
		path_to_test = NULL;
		i++;
	}
	ft_free_tabs(tab_paths, size_tab(tab_paths));
	return (NULL);
}

void	command_not_found(char *cmd)
{
	char	*msg_err;
	char	*tmp;

	tmp = ft_strjoin("pipex: command not found: ", cmd);
	msg_err = ft_strjoin(tmp, "\n");
	ft_putstr_fd(msg_err, 2);
	free(tmp);
	free(msg_err);
}

void	error_msg(char *file_name)
{
	char	*msg_sys;
	char	*msg_err;
	char	*tmp;

	msg_sys = strerror(errno);
	tmp = ft_strjoin("pipex: ", msg_sys);
	msg_err = ft_strjoin(tmp, ": ");
	free(tmp);
	tmp = NULL;
	tmp = ft_strjoin(msg_err, file_name);
	free(msg_err);
	ft_putstr_fd(tmp, 2);
	ft_putstr_fd("\n", 2);
	free(tmp);
}

static void handle_open_err(int fd, char *cmd)
{
	close(fd);
	error_msg(cmd);
	exit(EXIT_FAILURE);
}

static void handle_cmd_err(int fd, char **tab_cmd)
{
	command_not_found(tab_cmd[0]);
	ft_free_tabs(tab_cmd, size_tab(tab_cmd));
	close(fd);
}
/*
static void handle_dup_err(int fd, char **tab_cmd,)
{


}*/

void	child_process(int *pipefd, char *argv[], char *env[])
{
	int		fd_infile;
	char	*path_cmd1;
	char	**tab_cmd1;

	close(pipefd[0]);
	fd_infile = open(argv[1], O_RDONLY, 0644);
	if (fd_infile == -1)
		handle_open_err(pipefd[1], argv[1]);

	tab_cmd1 = ft_split(argv[2], ' ');
	path_cmd1 = cmd_exists(tab_cmd1[0], env);
	if (path_cmd1 == NULL)
	{
		handle_cmd_err(pipefd[1], tab_cmd1);
		return ;
	}
	if (dup2(fd_infile, 0) == -1 || dup2(pipefd[1], 1) == -1)
	{
		free(path_cmd1);
		ft_free_tabs(tab_cmd1, size_tab(tab_cmd1));
		close(fd_infile);
		close(pipefd[1]);
		perror("pipex");
		exit(EXIT_FAILURE);
	}
	close(fd_infile);
	if (execve(path_cmd1, tab_cmd1, env) == -1)
	{
		free(path_cmd1);
		ft_free_tabs(tab_cmd1, size_tab(tab_cmd1));
		close(pipefd[1]);
		perror("pipex");
		exit(EXIT_FAILURE);
	}
	close(pipefd[1]);
}

void	parent_process(int *pipefd, char *argv[], char *env[])
{
	int		fd_outfile;
	char	*path_cmd2;
	char	**tab_cmd2;

	close(pipefd[1]);
	fd_outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_outfile == -1)
	{
		close(pipefd[0]);
		error_msg(argv[1]);
		exit(EXIT_FAILURE);
	}
	tab_cmd2 = ft_split(argv[3], ' ');
	path_cmd2 = cmd_exists(tab_cmd2[0], env);
	if (path_cmd2 == NULL)
	{
		command_not_found(tab_cmd2[0]);
		free(tab_cmd2);
		close(pipefd[0]);
		exit(127);
	}
	if (dup2(pipefd[0], 0) == -1 || dup2(fd_outfile, 1) == -1)
	{
		free(tab_cmd2);
		ft_free_tabs(tab_cmd2, size_tab(tab_cmd2));
		close(pipefd[0]);
		perror("pipex");
		exit(EXIT_FAILURE);
	}
	close(fd_outfile);
	if (execve(path_cmd2, tab_cmd2, env) == -1)
	{
		//free(tab_cmd2);
		//ft_free_tabs(tab_cmd2, size_tab(tab_cmd2));
		close(pipefd[0]);
		perror("pipex");
		exit(EXIT_FAILURE);
	}
	close(pipefd[0]);
}
