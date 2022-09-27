#include "FileUtil.h"
#include <cassert> // assert
#include <cstring> // strerror

namespace TCB
{

FileUtil::AppendFile::AppendFile(const char* filename)
: fp_(fopen(filename, "a")), written_bytes_(0)
{
    assert(fp_);
    setbuffer(fp_, buffer_, sizeof(buffer_));
}

FileUtil::AppendFile::~AppendFile() {
    fclose(fp_);
}

// fputs_unlocked version, simpler but slower
// void FileUtil::AppendFile::append(const char* logline, size_t len) {
//     // speed : fwrite_unlocked > fputs_unlocked > fwrite > fputs > ofstream
//     if (fputs_unlocked(logline, fp_) < 0) {
//         int err = ferror(fp_);
//         fprintf(stderr, "AppendFile::append() failed %s\n", strerror(err));
//     } else {
//         written_bytes_ += len;
//     }
// }

void FileUtil::AppendFile::append(const char* logline, size_t len) {
    // speed : fwrite_unlocked > fputs_unlocked > fwrite > fputs > ofstream
    size_t written = 0;

    while (written != len)
    {
        size_t remain = len - written;
        size_t n = fwrite_unlocked(logline, 1, len, fp_);
        if (n != remain)
        {
            int err = ferror(fp_);
            if (err)
            {
                fprintf(stderr, "AppendFile::append() failed %s\n", strerror(err));
                break;
            }
        }
        written += n;
    }
}

void FileUtil::AppendFile::flush() {
    fflush(fp_);
}

} // namespace TCB
