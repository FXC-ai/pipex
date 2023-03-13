/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcoindre <fcoindre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:50:50 by fcoindre          #+#    #+#             */
/*   Updated: 2023/03/13 10:55:27 by fcoindre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
		handle_dup_err(fd_infile, pipefd[1], tab_cmd1, path_cmd1);
	close(fd_infile);
	if (execve(path_cmd1, tab_cmd1, env) == -1)
		handle_exec_err(pipefd[1], tab_cmd1, path_cmd1);
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
		handle_open_err(pipefd[0], argv[4]);
	tab_cmd2 = ft_split(argv[3], ' ');
	path_cmd2 = cmd_exists(tab_cmd2[0], env);
	if (path_cmd2 == NULL)
	{
		handle_cmd_err(pipefd[0], tab_cmd2);
		exit(127);
	}
	if (dup2(pipefd[0], 0) == -1 || dup2(fd_outfile, 1) == -1)
		handle_dup_err(fd_outfile, pipefd[0], tab_cmd2, path_cmd2);
	close(fd_outfile);
	if (execve(path_cmd2, tab_cmd2, env) == -1)
		handle_exec_err(pipefd[0], tab_cmd2, path_cmd2);
	close(pipefd[0]);
}
