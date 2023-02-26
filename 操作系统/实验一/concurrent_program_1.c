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

int main() {
    int i = 0, j = 0;
    pid_t pid;
    // 打印当前的父进程信息
    printf("Hello from Parent Process, PID is %d.\n", getpid());
    // 创建一个新的子进程，子进程会复制父进程的数据和堆栈空间
    pid = fork();
    
    // 如果pid == 0，那么说明与当前进程相同
    // 如果pid == -1，结束
    if (pid == 0) {
        sleep(1);
        for(int i = 0; i < 3; i++) {
            printf("Hello from Child Process %d. %d times\n", getpid(), i + 1);
            sleep(1);
        }
    } else if (pid != -1) {
        tprintf("Parent forked one child process--%d.\n", pid);
        tprintf("Parent is waiting for child to exit.\n");
        waitpid(pid, NULL, 0);
        tprintf("Child Process has exited.\n");
        tprintf("Parent had exited.\n");
    } else tprintf("Everything was done without error.\n");

    return 0;
}
