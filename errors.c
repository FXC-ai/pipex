/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcoindre <fcoindre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 10:47:32 by fcoindre          #+#    #+#             */
/*   Updated: 2023/03/13 10:48:06 by fcoindre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	handle_open_err(int fd_p, char *cmd)
{
	close(fd_p);
	error_msg(cmd);
	exit(EXIT_FAILURE);
}

void	handle_cmd_err(int fd_p, char **tab_cmd)
{
	command_not_found(tab_cmd[0]);
	ft_free_tabs(tab_cmd, size_tab(tab_cmd));
	close(fd_p);
}

void	handle_dup_err(int fd_f, int fd_p, char **tab_cmd, char *path_cmd)
{
	free(path_cmd);
	ft_free_tabs(tab_cmd, size_tab(tab_cmd));
	close(fd_f);
	close(fd_p);
	perror("pipex");
	exit(EXIT_FAILURE);
}

void	handle_exec_err(int fd_p, char **tab_cmd, char *path_cmd)
{
	free(path_cmd);
	ft_free_tabs(tab_cmd, size_tab(tab_cmd));
	close(fd_p);
	perror("pipex");
	exit(EXIT_FAILURE);
}
