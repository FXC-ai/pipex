/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcoindre <fcoindre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:53:50 by fcoindre          #+#    #+#             */
/*   Updated: 2023/03/08 15:56:18 by fcoindre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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