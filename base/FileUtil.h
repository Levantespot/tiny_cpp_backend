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

    void append(const char* logline, size_t len);
    void flush();

    ~AppendFile();
private:
    size_t wirtenBytes () const { return written_bytes_; }
private:
    FILE* fp_;
    char buffer_[64 * 1024]; // 64kb default
    size_t written_bytes_;
};

    } // namespace FileUtil

} // namespace TCB

#endif // #ifndef TCB_BASE_FILEUTIL_H