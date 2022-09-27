#include "../FileUtil.h"
#include "../Logger.h"

static TCB::FileUtil::AppendFile *p_af;

void append(const char* line, int len) {
    p_af->append(line, len);
}
void flush() {
    p_af->flush();
}

int main() {
    TCB::FileUtil::AppendFile af("logfile.log");
    p_af = &af;
    
    TCB::Logger::setOutput(append);
    TCB::Logger::setFlush(flush);

    LOG_INFO << "Hello" ;

    LOG_INFO << "my friend." ;

    return 0;
}