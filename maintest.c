#include "pipex.h"

int main()
{
    int fd_infile;
    fd_infile = open("file_in", O_RDONLY | O_CREAT, 0644);
    printf("%d\n",fd_infile);
    return 0;
}