#include "pipex.h"
#include <string.h>
#include <stdio.h>

/*
void command_not_found(char *cmd)
{
    char *msg_err;
    char *tmp;

    tmp = ft_strjoin("pipex: command not found: ", cmd);

    msg_err = ft_strjoin(tmp, "\n");

    ft_putstr_fd(msg_err,2);

    free(tmp);
    free(msg_err);
}*/


void permission_denied(char *file_name)
{
    char *msg_sys;
    char *msg_err;
    char *tmp;

    msg_sys = strerror(EACCES);
    tmp = ft_strjoin("pipex: ", msg_sys);
    msg_err = ft_strjoin(tmp, ": ");
    free(tmp);
    tmp = NULL;


    tmp = ft_strjoin(msg_err, file_name);
    free(msg_err);

    ft_putstr_fd(tmp,2);
    ft_putstr_fd("\n",2);

    free(tmp);
}

int main()
{
    permission_denied("nom_du_fichier");
    return 0;
}