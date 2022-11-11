#include <unistd.h>

#include <iostream>
#include <thread>

#include "net/EventLoop.h"

void func_in_thread() {
  TCB::net::EventLoop el;

  el.loop();

  std::cout << "thread: tid = " << gettid() << '\n';
}

int main() {
  TCB::net::EventLoop el;

  std::cout << "main: pid = " << getpid() << '\n'
            << "CurrentThread::tid() = " << TCB::CurrentThread::tid() << '\n';

  el.loop();

  std::thread t(func_in_thread);
  t.join();

  TCB::net::EventLoop el2;  // error
}