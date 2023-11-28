#include <sys/types.h>
#include <unistd.h>

int main(){
    pid_t pid;
    
    printf("calling fork \n");
    pid = fork();
    if(pid == 0)
        printf("I'm the child process\n");
    else if(pid>0)
        printf("I'm the parent process\n");
    else
        printf("fork failed\n");
}
