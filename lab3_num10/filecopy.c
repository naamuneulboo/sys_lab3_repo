#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define SHM_SIZE 1024  // 공유 메모리 크기

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

// 세마포어 초기화 함수
int set_semvalue(int sem_id) {
    union semun sem_union;
    sem_union.val = 1;
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1) return 0;
    return 1;
}

// 세마포어 삭제 함수
void del_semvalue(int sem_id) {
    union semun sem_union;
    semctl(sem_id, 0, IPC_RMID, sem_union);
}

// 세마포어 P 연산 함수
int semaphore_p(int sem_id) {
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1;  // P 연산
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_p failed\n");
        return 0;
    }
    return 1;
}

// 세마포어 V 연산 함수
int semaphore_v(int sem_id) {
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1;  // V 연산
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_v failed\n");
        return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    int sem_id;
    key_t key;
    int shmid;
    char *shm, *s;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source file> <destination file>\n", argv[0]);
        exit(1);
    }

    char *source_file = argv[1];
    char *dest_file = argv[2];

    // 공유 메모리 생성
    key = 5678;
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // 공유 메모리를 프로세스의 메모리 공간에 붙인다.
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    // 세마포어 생성
    sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);
    if (!set_semvalue(sem_id)) {
        fprintf(stderr, "Failed to initialize semaphore\n");
        exit(EXIT_FAILURE);
    }

    int fd_source, fd_dest;
    char buffer[SHM_SIZE];
    ssize_t bytes;

    if ((fd_source = open(source_file, O_RDONLY)) == -1) {
        perror("open source file");
        exit(1);
    }

    if ((fd_dest = open(dest_file, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
        perror("open destination file");
        exit(1);
    }

    if (fork()) {  // 부모 프로세스
        while ((bytes = read(fd_source, buffer, SHM_SIZE)) > 0) {
            semaphore_p(sem_id);
            memcpy(shm, buffer, bytes);
            semaphore_v(sem_id);
        }
        close(fd_source);
        wait(NULL);
    } else {  // 자식 프로세스
        while (1) {
            semaphore_p(sem_id);
            bytes = strlen(shm);
            if (bytes == 0) break;
            write(fd_dest, shm, bytes);
            memset(shm, 0, SHM_SIZE);
            semaphore_v(sem_id);
        }
        close(fd_dest);
    }

    del_semvalue(sem_id);
    shmctl(shmid, IPC_RMID, 0);

    return 0;
}


