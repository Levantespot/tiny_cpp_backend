#include "../EventLoop.h"
#include <stdio.h>

TCB::net::EventLoop* g_loop;
int g_flag = 0;

void run4()
{
    printf("run4(): tid = %d, flag = %d\n", gettid(), g_flag);
    g_loop->quit();
}

void run3()
{
    printf("run3(): tid = %d, flag = %d\n", gettid(), g_flag);
    g_loop->queueInLoop(run4);
    g_flag = 3;
}

void run2()
{
    printf("run2(): tid = %d, flag = %d\n", gettid(), g_flag);
    g_loop->queueInLoop(run3);
}

void run1()
{
    g_flag = 1;
    printf("run1(): tid = %d, flag = %d\n", gettid(), g_flag);
    g_loop->runInLoop(run2);
    g_flag = 2;
}

int main()
{
    printf("main(): tid = %d, flag = %d\n", gettid(), g_flag);

    TCB::net::EventLoop loop;
    g_loop = &loop;

    loop.runInLoop(run1);
    loop.loop();
    printf("main(): tid = %d, flag = %d\n", gettid(), g_flag);
}
