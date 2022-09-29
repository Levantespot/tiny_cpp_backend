#ifndef TCB_BASE_FILEUTIL_H
#define TCB_BASE_FILEUTIL_H

#include "noncopyable.h"
#include <stdio.h> // FILE, size_t, 

namespace TCB
{

namespace FileUtil
{
    // 封装最基本的文件增添功能
    // Note: thread unsafe
    class AppendFile : noncopyable {
    public:
        explicit AppendFile(const char* filename);
        ~AppendFile();

        // 不保证线程安全性，由上层程序保证
        // 经测试，在多线程的情况下，若一开始不存在日志文件，有概率会丢失一条信息
        void append(const char* logline, size_t len); 
        void flush();

        size_t writenBytes () const { return written_bytes_; }
    private:
        FILE* fp_;
        static char buffer_[64 * 1024]; // 64kb default
        size_t written_bytes_;
    };

} // namespace FileUtil

} // namespace TCB

#endif // #ifndef TCB_BASE_FILEUTIL_H