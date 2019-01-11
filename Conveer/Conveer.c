/*
 * Аналог башевского: ls -la | grep a
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void first_command(int fd[2])
{
    close (fd[0]);
    close(STDOUT_FILENO);
    dup2 (fd[1], STDOUT_FILENO);
    close (fd[1]);
    
    if (execl("/bin/ls", "ls", "-la", NULL)<0)
    {
        perror("execl");
        exit(2);
    }
}

void second_command(int fd[2])
{
    close (fd[1]);
    close(STDIN_FILENO);
    dup2 (fd[0], STDIN_FILENO);
    close(fd[0]);
    
    if (execl("/bin/grep", "grep", "a", NULL)<0)
    {
        perror("execl");
        exit(3);
    }
}

int main(int argc, char **argv)
{
    int fd[2];
    pid_t pid;

    if (pipe (fd)!=0)
        perror ("pipe");

    pid = fork();
    if( pid == -1)
    {
        perror("fork");
        exit(1);
    }
    else if (pid == 0)
    {
        first_command(fd);
        exit (1);
    }

    pid = fork();
    if( pid == -1)
    {
        perror("fork");
        exit(1);
    }
    else if (pid == 0)
    {
        second_command(fd);
    }

    close (fd[0]);
    close (fd[1]);

    wait(NULL);

    return 0;
}

