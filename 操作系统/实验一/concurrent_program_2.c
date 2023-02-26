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
    // 如果pid > 0，说明当前为父进程
    if (pid == 0) {
        sleep(5);
        tprintf("Hello from Child Process!\n");
        tprintf("I am calling exec.\n");
        execl("/bin/ps", "-a", NULL);
        //execl("/bin/ls", "-l", "/etc", NULL);
        tprintf("You should never see this because the child is already gone.\n");
    } else if (pid != -1) {
        tprintf("Hello from Parent, PID %d.\n", getpid());
        sleep(1);
        tprintf("Parent forked process %d.\n", pid);
        sleep(1);
        tprintf("Parent is waiting for child to exit.\n");
        waitpid(pid, NULL, 0);
        tprintf("Parent had exited.\n");
    } else tprintf("Everything was done without error.\n");

    return 0;
}
