#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>

#define BUFSIZE 16
#define QKEY (key_t)0111

struct msgq_data{
    long type;
    char text[BUFSIZE];
};

struct msgq_data data;

int qid;

void *send_msg(void *arg){
    data.type=1;
    while(fgets(data.text, BUFSIZE, stdin)){
        if(msgsnd(qid, &data, strlen(data.text),0)==-1){
            perror("msgsnd failed");
            exit(1);
        }
    }
}

void *recv_msg(void *arg){
    while(1){
        if(msgrcv(qid,&data,BUFSIZE,0,0)==-1){
            perror("msgrcv failed");
            exit(1);
        }
        printf("received from message queue: %s", data.text);
    }
}

int main(){
    pthread_t send_thread, recv_thread;
    
    if((qid=msgget(QKEY, IPC_CREAT|0666))==-1){
        perror("msgget failed");
        exit(1);
    }
    
    pthread_create(&send_thread, NULL,send_msg,NULL);
    pthread_create(&recv_thread, NULL,recv_msg,NULL);
    
    pthread_join(send_thread,NULL);
    pthread_join(recv_thread,NULL);
    
    if(msgctl(qid, IPC_RMID,0)==-1){
        perror("msgctl failed");
        exit(1);
    }
    
    return 0;
}
