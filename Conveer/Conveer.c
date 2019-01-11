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
    dup2 (fd[1], STDOUT_FILENO);
    close (fd[0]);
    execl("/bin/ls", "ls", "-la", NULL);
}

void second_command(int fd[2])
{
    dup2 (fd[0], STDIN_FILENO);
    close (fd[1]);
    execl("/bin/grep", "grep", "a", NULL);
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
