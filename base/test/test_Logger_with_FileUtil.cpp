#include "../FileUtil.h"
#include "../Logger.h"

int main() {
    TCB::FileUtil::AppendFile af("logfile.log");

    std::function<void (const char*, size_t)> append2file = \
        std::bind(&TCB::FileUtil::AppendFile::append, &af, std::placeholders::_1, std::placeholders::_2);
    std::function<void ()> flush2file = std::bind(&TCB::FileUtil::AppendFile::flush, &af);
    
    TCB::Logger::setOutput(append2file);
    TCB::Logger::setFlush(flush2file);

    LOG_INFO << "Hello" ;
    LOG_INFO << std::string(1000, '-') ;
    LOG_INFO << "my friend." ;
    LOG_INFO << std::string(1000 * 1000, '-') ;

    return 0;
}