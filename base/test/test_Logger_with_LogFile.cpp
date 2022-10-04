#include "../LogFile.h"
#include "../Logger.h"
#include <string>
#include <unistd.h>
#include <iostream>

int main() {
    TCB::LogFile log_file("logfile", 1024);

    // 适配函数
    std::function<void (const char*, size_t)> append2file = \
        std::bind(&TCB::LogFile::append, &log_file, std::placeholders::_1, std::placeholders::_2);
    std::function<void ()> flush2file = std::bind(&TCB::LogFile::flush, &log_file);
    
    // 绑定函数
    TCB::Logger::setOutput(append2file);
    TCB::Logger::setFlush(flush2file);

    LOG_INFO << "1" ;
    LOG_INFO << std::string(2000, 'H');
    LOG_INFO << "2" ;
    std::cout << "Wait for 5 seconds\n" ;
    sleep(5); // 保证两次时间不同，不然可能不会滚动文件
    LOG_INFO << std::string(2000, '-');
    LOG_INFO << "2" ;
    LOG_INFO << std::string(4*1024, 'H') ;
    LOG_INFO << "3" ;

    return 0;
}