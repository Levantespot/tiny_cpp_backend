#include "../LogFile.h"
#include "../Logger.h"
#include <string>
#include <unistd.h>
#include <iostream>

void wait() {
    // 保证两次时间不同，不然可能不会滚动文件
    std::cout << "Wait for 1 seconds\n" ;
    sleep(1);
}

int main() {
    TCB::LogFile log_file("logfile", 1024);

    // 适配函数
    std::function<void (const char*, std::size_t)> append2file = \
        std::bind(&TCB::LogFile::append, &log_file, std::placeholders::_1, std::placeholders::_2);
    std::function<void ()> flush2file = std::bind(&TCB::LogFile::flush, &log_file);
    
    // 绑定函数
    TCB::Logger::setOutput(append2file);
    TCB::Logger::setFlush(flush2file);

    LOG_INFO << "1" ;
    wait(); 
    LOG_INFO << std::string(850, '2');
    wait(); 
    for (int i = 0; i < 'z' - 'a'; i++) {
        LOG_INFO << 'a' + i;
        wait();
    }
    LOG_INFO << "3" ;
    wait();
    LOG_INFO << "4" ;
    wait();
    LOG_INFO << "5" ;
    wait();
    LOG_INFO << "6" ;

    return 0;
}