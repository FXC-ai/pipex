#include "libft/libft.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>


int main(int argc, char *argv[], char *env[])
{
	
	pid_t pid;
	int status, fd;
	int result = 0;
	char str[2048];

	pid = fork();

	if (pid == 0)
	{
		if (argc > 1)
		{
			fd = open("text.txt", O_RDWR | O_CREAT, 0644);
			printf("fils fd = %d\n", fd);
			dup2(fd,1); //alors la, c'est simple : le file descriptor 1 pointe desormais sur fd
			result = execve(argv[1], argv + 1, env);
			close(fd);
			if (result == -1)
			{
				perror("execve");
			}

		}
	}
	else
	{
		waitpid(-1, &status, 0);
		printf("Processus Papa %d\n", result);

		fd = open("text.txt", O_RDWR | O_CREAT, 0644);

		read(fd, str, 2048);
		
		printf("str = %s\n", str);

	}
	return 0;
}