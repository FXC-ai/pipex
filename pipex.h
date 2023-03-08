/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcoindre <fcoindre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 16:07:31 by fcoindre          #+#    #+#             */
/*   Updated: 2023/03/08 16:07:33 by fcoindre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

#include "libft/libft.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>

char *cmd_exists(char *cmd, char *env[]);
void	ft_free_tabs(char **tab, int h);
int size_tab(char **tab);
char **create_tab_paths(char *env[]);
char *create_path_cmd(char *path, char *cmd);

char *cmd_exists(char *cmd, char *env[]);
void    child_process(int *pipefd, char *argv[], char *env[]);
void    parent_process(int *pipefd, char *argv[], char *env[]);


#endif