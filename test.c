#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[], char *env[])
{
	
	pid_t pid;
	int i, status, fd;
	int result;
	char str[2048];

	pid = fork();

	if (pid == 0)
	{
		if (argc > 1)
		{
			fd = open("test.txt", O_RDWR | O_CREAT, 0644);
			printf("fils fd = %d\n", fd);
			dup2(fd,1); //alors la, c'est simple : le file descriptor 1 pointe desormais sur fd
			close(fd);
			result = execve(argv[1], argv + 1, env);
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

		fd = open("test.txt", O_RDWR | O_CREAT, 0644);

		read(fd, str, 2048);
		
		printf("str = %s\n", str);

	}
	return 0;
}
/*
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
int main(int argc, char *argv[], char *env[])
{
  pid_t         pid;
  int           pipe_fd[2];

  if (argc < 2)
    return 1;

  if (pipe(pipe_fd) == -1)
    {
      perror("pipe");
      return 1;
    }

  if ((pid = fork()) == -1)
    {
      perror("fork");
      return 1;
    }
 	else if (pid == 0)
    {
		printf("pipe 0 = %d | pipe 1 = %d", pipe_fd[0], pipe_fd[1]);

      	close(pipe_fd[0]);
      	if (dup2(pipe_fd[1], 1) == -1)
	  	{
      		perror("dup2");
	  	}
	  	else if (execve(argv[1], argv + 1, env) == -1)
      	{
	  		perror("execve");
		}
	  return 1;
    }
  else
    {
      char      buffer[1024];
      int       ret;
      int       status;

		printf("papa pipe 0 = %d | pipe 1 = %d", pipe_fd[0], pipe_fd[1]);
      close(pipe_fd[1]);

      while ((ret = read(pipe_fd[0], buffer, 1023)) != 0)
      {
        printf("%d\n", ret);
        buffer[ret] = 0;
        printf("%s\n", buffer);
      }
    }

  return 0;
}
*/
/*
int main(void)
{

	pid_t pid;
	int	pipefd[2];
	int status;
	char buf[5];
	
	char *chaine = "Test";

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	printf("pipefd[0] : %d | pipefd[1] : %d\n", pipefd[0], pipefd[1]);

	pid = fork();
	if (pid == 0)
	{
		close(pipefd[0]);
		printf("Check write : %zd\n", write(pipefd[1], chaine, 5));
		close(pipefd[1]);
		//return (1);
	}
	else
	{
		wait(&status);
		close(pipefd[1]);
		printf("Check read : %zd\n", read(pipefd[0], buf, 5));
		printf("Message recu du processus fils : %s", buf);
		close(pipefd[0]);
	}
	

	return (0);
}
*/
/*
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[], char *env[])
{
  pid_t          pid;
  int            status;
  int            pipe_fd[2];

  if (argc < 2)
    return 1;

  if (pipe(pipe_fd) == -1)
    {
      perror("pipe");
      return 1;
    }

  if ((pid = fork()) == -1)
    {
      perror("fork");
      return 1;
    }
  else if (pid == 0)
    {
      close(pipe_fd[0]);
      write(pipe_fd[1], argv[1], strlen(argv[1]));
      close(pipe_fd[1]);
      return 1;
    }
  else
    {
      char          buffer[1024];
      int           ret;

      close(pipe_fd[1]);
      while ((ret = read(pipe_fd[0], buffer, 1023)))
        {
          buffer[ret] = 0;
          printf("Message recu : %s\n", buffer);
        }
      close(pipe_fd[0]);
    }

  return 0;
}
*/
/*
int main(int argc, char *argv[], char *env[])
 {

	int fd, fd_copy, status;
	int access_result;
	int nb_read;
	char	buf[15];
	pid_t pid;


	printf("\n--------------- test fork ---------------\n");
	if ((pid = fork()) == -1)
	{
		perror("fork");
		return 1;
	}
	else if (pid == 0)
	{
    	printf("\033[31m Je suis le fils. %d\033[0m \n", getpid());

		printf("\n--------------- test execve ---------------\n");
		if (argc > 1)
		{
			if (execve(argv[1], argv + 1, env) == -1)
			{
				perror("execve");
			}
		}
	}
	else
	{

		printf("\n--------------- test wait ---------------\n");
		wait(&status);

    	printf("Je suis le pere, et mon pid = %d. Le pid de mon fils = %d\n", getpid(), pid);
	
		printf("\n--------------- test access ---------------\n");
		access_result = access("helo.txt", F_OK);
		printf("access result = %d\n", access_result);


		printf("\n--------------- test strerror ---------------\n");
		printf("%d : %s\n",errno,strerror(errno));
		perror("access");
		

		printf("\n--------------- test dup ---------------\n");
		fd = open("hello.txt", O_RDONLY);
		fd_copy = dup(fd);

		nb_read = read(fd, buf, 10);
		buf[nb_read] = '\0';
		printf("Voici les caracteres lus sur le fd %d : %s\n", fd, buf);

		nb_read = read(fd_copy, buf, 10);
		buf[nb_read] = '\0';
		printf("Voici les caracteres lus sur le fd %d : %s\n", fd_copy, buf);

		close(fd);
		close(fd_copy);

		printf("\n--------------- test dup2 ---------------\n");
		fd = open("hello.txt", O_RDONLY);
		fd_copy = dup2(fd, 7);

		nb_read = read(fd, buf, 10);
		buf[nb_read] = '\0';
		printf("Voici les %d caracteres lus sur le fd %d : %s\n", nb_read, fd_copy, buf);

		printf("\n--------------- test ---------------\n");
		write(1,"Bonjour",7);

		close(fd);
		close(fd_copy);




		printf("\n--------------- test exit ---------------\n");
		exit(0);
	}


    return 0;
}
*/
