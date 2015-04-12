#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
	int fd[2];
	if (pipe(fd) < 0)
	{
		printf("Cannot create pipe\n");
		return 1;
	}
	if (fork()==0)
	{
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
		execlp("cat", "cat", "/etc/passwd", NULL);
		return 1;
	}
	if (fork()==0)
	{
		dup2(fd[0], 0);
		close(fd[0]);
		close(fd[1]);
		execlp("wc", "wc", NULL);
		return 1;
	}
	close(fd[0]);
	close(fd[1]);
	wait( (int*) 0);
	return 0;
}

