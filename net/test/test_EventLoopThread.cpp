#include "../EventLoop.h"
#include "../EventLoopThread.h"
#include <stdio.h>

void runInThread()
{
  printf("runInThread(): pid = %d, tid = %d\n",
         getpid(), TCB::CurrentThread::tid());
}

int main()
{
  printf("main(): pid = %d, tid = %d\n",
         getpid(), TCB::CurrentThread::tid());

  TCB::net::EventLoopThread loopThread;
  TCB::net::EventLoop* loop = loopThread.startLoop();
  loop->runInLoop(runInThread);
  sleep(3);
  loop->quit();

  printf("exit main().\n");
}
