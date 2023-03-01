#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[], char *env[])
 {

	int fd, fd_copy;
	int access_result;
	int nb_read;
	char	buf[15];


	if (argc > 1)
	{
		if (execve(argv[1], argv + 1, env) == -1)
		{
			perror("execve");
		}
		
	}
	

	access_result = access("hello.txt", F_OK);

	printf("access result = %d\n", access_result);

	printf("%s\n",strerror(errno));
	perror("Pipex");
	

	fd = open("hello.txt", O_RDONLY);
	perror("Pipex 2");

	fd_copy = dup(fd);
	perror("Pipex 3");

	nb_read = read(fd, buf, 10);
	buf[nb_read] = '\0';

	printf("Voici les caracteres lus sur le fd %d : %s\n", fd, buf);

	nb_read = read(fd_copy, buf, 10);
	buf[nb_read] = '\0';

	printf("Voici les caracteres lus sur le fd %d : %s\n", fd_copy, buf);


    return 0;
}