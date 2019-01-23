#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/errno.h>

#define QTYPE_FOR_REQUEST 1
#define QTYPE_FOR_ANSWER 2

#define REQUEST_STR "Hello"
#define ANSWER_STR "Hi"

#define MAX_SEND_SIZE 100

struct mymsgbuf
{
    long mtype;
    char mtext[MAX_SEND_SIZE];
};

int create_queue()
{
    key_t key;
    int new_msgqid;
    key = ftok(".", 's');
    if((new_msgqid = msgget(key, IPC_CREAT|0666)) == -1)
    {
        perror("msgget");
        exit(1);
    }
    printf("Queue created; ID = %d\n", new_msgqid);
    return new_msgqid;
}

void send_message(int qid, struct mymsgbuf *qbuf, long type, char *text)
{
    qbuf->mtype = type;
    strcpy(qbuf->mtext, text);

    if((msgsnd(qid, (struct msgbuf *)qbuf,
               strlen(qbuf->mtext)+1, 0)) == -1)
    {
        perror("msgsnd");
        exit(1);
    }
}

void read_message(int qid, struct mymsgbuf *qbuf, long type)
{
    int rc;
    qbuf->mtype = type;
    rc = msgrcv(qid, (struct msgbuf *)qbuf, MAX_SEND_SIZE, type, 0);
    if (rc < 0)
    {
        perror("msgrcv");
        printf("msgrcv failed, rc=%d\n", rc);
        exit(1);
    }
}

void remove_queue(int msgqid)
{
    int rc;
    if ((rc = msgctl(msgqid, IPC_RMID, NULL)) < 0)
    {
        perror("msgctl");
        printf("msgctl (return queue) failed, rc=%d\n", rc);
        exit(1);
    }
    else
    {
        printf("Queue deleted\n");
    }
}

int main(int argc, char *argv[])
{
    pid_t pid;
    struct mymsgbuf qbuf;
    int msgqid;

    msgqid=create_queue();

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }
    else if (pid == 0)
    {
        send_message(msgqid, (struct mymsgbuf *)&qbuf,QTYPE_FOR_REQUEST, REQUEST_STR);
        printf("CHILD: send message:%s\n",REQUEST_STR);
        read_message(msgqid, &qbuf, QTYPE_FOR_ANSWER);
        printf("CHILD: get type %ld message: %s\n",qbuf.mtype,qbuf.mtext);
        exit(0);
    }

    //родительский процесс
    read_message(msgqid, &qbuf, QTYPE_FOR_REQUEST);
    printf("PARENT: get type %ld message: %s\n",qbuf.mtype,qbuf.mtext);
    send_message(msgqid, (struct mymsgbuf *)&qbuf, QTYPE_FOR_ANSWER, ANSWER_STR);
    printf("PARENT: send message: %s\n", ANSWER_STR);

    wait(NULL);
    
    //удаление созданной очереди
    remove_queue(msgqid);
    
    return 0;
}

