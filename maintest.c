#include "pipex.h"


void command_not_found(char *cmd)
{
    char *msg_err;
    char *tmp;

    tmp = ft_strjoin("pipex: command not found: ", cmd);

    msg_err = ft_strjoin(tmp, "\n");

    ft_putstr_fd(msg_err,2);

    free(tmp);
    free(msg_err);
}


int main()
{
    command_not_found("whateverr");
    return 0;
}