/*
    TREE_OF_PROCESSES
         1
      /    \
     2      3
     |     / \
     4    5   6
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void pidprint()
{
    printf("Process pid=%d, ppid=%d\n", getpid(), getppid());
}

int main(int argc, char** argv)
{
    int i;
    pid_t pid;
    printf("parent:");
    pidprint();
    
    for(i = 0; i < 2; i++)
    {
        pid = fork();
        if(pid == 0)
        {
            pidprint();
            if(i == 0)
            {
                pid = fork();
                if(pid == 0)
                {
                    pidprint();
                    exit(1);
                }
                else
                {
                    wait(NULL);
                    exit(1);
                }
            }

            if(i == 1)
            {
                pid = fork();
                if(pid == 0)
                {
                    pidprint();
                    exit(1);
                }

                pid = fork();
                if(pid == 0)
                {
                    pidprint();
                    exit(1);
                }
                else
                {
                    wait(NULL);
                    wait(NULL);
                    exit(1);
                }
            }
        }
        else
            wait(NULL);
    }

    return 0;
}
