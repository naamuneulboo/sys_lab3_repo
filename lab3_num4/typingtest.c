#include <stdio.h>
#include <termios.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#define PASSWORDSIZE 12

int main(void)
{
    int fd;
    int nread, cnt=0, errcnt=0;
    char ch;
    const char *texts[] = {"Stroms do not last forever.", "Lean on me, and together,", "we will wait for the clouds to pass."};
    int num_sentences = sizeof(texts) / sizeof(texts[0]);
    struct termios init_attr, new_attr;
    time_t start_time, end_time;

    fd = open(ttyname(fileno(stdin)), O_RDWR);
    tcgetattr(fd, &init_attr);
    new_attr = init_attr;
    new_attr.c_lflag &= ~ICANON;
    new_attr.c_lflag &= ~ECHO;
    new_attr.c_cc[VMIN] = 1;
    new_attr.c_cc[VTIME] = 0;

    if (tcsetattr(fd, TCSANOW, &new_attr) != 0) {
        fprintf(stderr, "터미널 속성을 설정할 수 없음.\n");
    }

    for(int i=0; i<num_sentences; i++) {
        cnt = 0;
        errcnt = 0;
        printf("다음 문장을 그대로 입력하세요.\n%s\n", texts[i]);
        time(&start_time);
        while ((nread=read(fd, &ch, 1)) > 0 && ch != '\n') {
            if (ch == texts[i][cnt++])
                write(fd, &ch, 1);
            else {
                write(fd, "", 1);
                errcnt++;
            }
        }
        time(&end_time);
        double typing_speed = (double)cnt / difftime(end_time, start_time) * 60;
        printf("타이핑 오류의 횟수는 %d, 평균 분당 타자 수는 %.2f\n", errcnt, typing_speed);
    }

    tcsetattr(fd, TCSANOW, &init_attr);
    close(fd);
    return 0;
}
