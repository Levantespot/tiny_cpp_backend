#include <sys/timerfd.h>
#include "../Channel.h"
#include "../EventLoop.h"
#include <iostream>

TCB::net::EventLoop* g_loop;

void timeout() {
    std::cout << "Timeout!\n" ;
    g_loop->quit();
}

int main() {
    TCB::net::EventLoop loop;
    g_loop = &loop;

    int timerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);

    TCB::net::Channel ch(&loop, timerfd);
    ch.setReadCallBack(timeout);
    ch.enableReading();

    struct itimerspec howlong = 
    {   
        { 0, 0 },
        { 5, 0 }
    };
    // struct itimerspec howlong = 
    // {   
    //     { 5, 0 }, // interval (seconds and Nanoseconds), Set to {0, 0} if you need a one-shot timer
    //     { 4, 0 }  // intial (seconds and Nanoseconds)
    // };
    timerfd_settime(timerfd, 0, &howlong, NULL);

    std::cout << "Start:\t " << time(NULL) << '\n';
    loop.loop();
    std::cout << "End:\t " << time(NULL) << '\n';

    close(timerfd);
    return 0;
}