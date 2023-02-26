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
    printf("%02d:%02d:%02d: %05d| ", tstruct->tm_hour, tstruct->tm_min, tstruct->tm_sec, getpid());
    va_start(args, fmt);
    return vprintf(fmt, args);
}

void exec(int pid) {
    if (pid == 0) {
        sleep(1);
        tprintf("Hello from Child! Child PID is %d.\n", getpid());
        tprintf("Child %d had exited.\n", getpid());
        sleep(1);
    } else if (pid != -1) {
        tprintf("Hello from Parrent, PID is %d.\n", getpid());
        tprintf("Parrent is waiting for child to exit.\n");
        waitpid(pid, NULL, 0);
        tprintf("Parrent had exited.\n");
    } else tprintf("Everything was done without error.\n");
}

int main() {
    pid_t pid;
    printf("Hello from Parrent Process, Parrent PID is %d.\n", getpid());
    // 创建两个子进程
    for(int i = 0; i < 2; i++) {
        pid = fork();
        if (pid == 0 || pid == -1) break;   // 如果当前线程是子线程，那么就跳出循环。否则就继续创建子线程。
    }
    exec(pid);
    return 0;
}