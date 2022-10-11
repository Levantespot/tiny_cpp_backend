#include "FileUtil.h"

namespace TCB
{

FileUtil::AppendFile::AppendFile(const char* filename)
  : fp_(fopen(filename, "a")),
    buffer_{0},
    written_bytes_(0)
{
    // assert(fp_);
    if (fp_ == nullptr) {
        int err = ferror(fp_);
        fprintf(stderr, "AppendFile::AppendFile() failed %s\n", strerror(err));
    }
    setbuffer(fp_, buffer_, sizeof(buffer_));
}

FileUtil::AppendFile::~AppendFile() {
    fclose(fp_);
}

// fputs_unlocked version, simpler but slower
// void FileUtil::AppendFile::append(const char* logline, std::size_t len) {
//     // speed : fwrite_unlocked > fputs_unlocked > fwrite > fputs > ofstream
//     if (fputs_unlocked(logline, fp_) < 0) {
//         int err = ferror(fp_);
//         fprintf(stderr, "AppendFile::append() failed %s\n", strerror(err));
//     } else {
//         written_bytes_ += len;
//     }
// }

void FileUtil::AppendFile::append(const char* logline, std::size_t len) {
    // speed : fwrite_unlocked > fputs_unlocked > fwrite > fputs > ofstream
    std::size_t written = 0;

    while (written != len)
    {
        std::size_t remain = len - written;
        std::size_t n = fwrite_unlocked(logline, 1, len, fp_);
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
    written_bytes_ += written;
}

void FileUtil::AppendFile::flush() {
    fflush(fp_);
}

} // namespace TCB
