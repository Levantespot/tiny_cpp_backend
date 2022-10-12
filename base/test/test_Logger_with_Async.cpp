#include "../AsyncLogging.h"
#include "../Logger.h"
#include <iostream>
#include <functional>
#include <string>
#include <unistd.h>

int main() {
    // 后端
    TCB::AsyncLogging alog("logfile", 2 * TCB::kLargeBuffer);
    TCB::Logger::setOutput(std::bind(&TCB::AsyncLogging::append, &alog, std::placeholders::_1, std::placeholders::_2));
    // TCB::Logger::setFlush(std::bind(&TCB::AsyncLogging::flush, &alog));

    LOG_INFO << "Hello" ;
    sleep(1);
    for (int i = 0; i < 1000; i++) {
        LOG_INFO << std::string(3000, '0');
    }
    sleep(1);
    for (int i = 0; i < 1000; i++) {
        LOG_INFO << std::string(3000, '1');
    }
    sleep(1);
    for (int i = 0; i < 2000; i++) {
        LOG_INFO << std::string(3000, '2');
    }
    sleep(1);
    for (int i = 0; i < 2000; i++) {
        LOG_INFO << std::string(3000, '3');
    }
    LOG_INFO << "FINISH" ;
    return 0;
}