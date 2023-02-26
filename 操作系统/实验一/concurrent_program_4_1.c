#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

// 按照格式重写printf函数，格式是时间 + fmt
int tprintf(const char* fmt, ...) {
    va_list args;
    struct tm *tstruct;
    time_t tsec;
    tsec = time(NULL);
    tstruct = localtime(&tsec);
    printf("%02d:%02d:%02d - PID:%05d| ", tstruct->tm_hour, tstruct->tm_min, tstruct->tm_sec, getpid());
    va_start(args, fmt);
    return vprintf(fmt, args);
}

int main() {
    pid_t pid;
    printf("Hello from Parrent Process, Parrent PID is %d.\n", getpid());
    // 直接创建10个子进程
    for(int i = 0; i < 10; i++) {
        pid = fork();
        if (pid == 0) {
            // child
            //sleep(1);
            tprintf("Hello from Child! Child PID is %d.\n", getpid());
            break;
        } else if (pid != -1) {
            // parrent
            tprintf("Now Parrent is waiting child %d...\n", pid);
            tprintf("Child %d had exited.\n", pid);
        } else tprintf("Everything was done without error.\n");
    }
    pause();
    return 0;
}