#include "base/LogFile.h"
// #include <cassert> // assert

namespace TCB {

LogFile::LogFile(const std::string& basename, std::size_t rollSize, bool threadSafe,
                 int flushInterval, int checkEveryN)
    : basename_(basename),
      rollSize_(rollSize),
      flushInterval_(flushInterval),
      checkEveryN_(checkEveryN),
      count_(0),
      mutex_(threadSafe ? new std::mutex : nullptr),
      startOfPeriod_(0),
      lastRoll_(0),
      lastFlush_(0) {
  // assert(basename.find('/') == string::npos);
  rollFile();  // 首次生成日志
}

void LogFile::append(const char* logline, std::size_t len) {
  if (mutex_) {
    std::lock_guard<std::mutex> lock(*mutex_);
    append_unlocked(logline, len);
  } else {
    append_unlocked(logline, len);
  }
}

void LogFile::append_unlocked(const char* logline, std::size_t len) {
  file_->append(logline, len);

  if (file_->writenBytes() > rollSize_) {
    // 写的字数超过 rollSize_ 阈值，滚动日志
    rollFile();
  } else {
    ++count_;
    if (count_ >= checkEveryN_) {  // 达到检查的条件
      count_ = 0;
      std::time_t now = time(nullptr);
      std::time_t thisPeriod = now / kRollPerSeconds_ * kRollPerSeconds_;
      if (thisPeriod != startOfPeriod_) {
        // 过了一天了，滚动日志
        rollFile();
      } else if (now - lastFlush_ > flushInterval_) {
        // 距上次刷缓存的时间超过阈值，刷缓存
        lastFlush_ = now;
        file_->flush();
      }
    }
  }
}

void LogFile::flush() {
  if (mutex_) {
    std::lock_guard<std::mutex> lock(*mutex_);
    file_->flush();
  } else {
    file_->flush();
  }
}

// 关闭文件 -> 新建新的文件 -> 更新状态（成员变量）
bool LogFile::rollFile() {
  std::time_t now = time(nullptr);

  if (now > lastRoll_) {
    std::string filename = createLogFileName(basename_, now);
    std::time_t startPeriod = now / kRollPerSeconds_ * kRollPerSeconds_;
    // 关闭文件 + 新建新的文件
    file_.reset(new FileUtil::AppendFile(filename.c_str()));
    // 更新状态
    lastRoll_ = now;
    lastFlush_ = now;
    startOfPeriod_ = startPeriod;

    return true;
  }
  return false;
}

std::string LogFile::createLogFileName(const std::string& basename, std::time_t now) {
  std::string filename;
  filename.reserve(basename.size() + 32);
  filename = basename;

  char timebuf[32];
  struct tm tm;
  gmtime_r(&now, &tm);
  strftime(timebuf, sizeof(timebuf), ".%Y%m%d-%H%M%S", &tm);
  filename += timebuf;

  filename += ".log";

  return filename;
}

}  // namespace TCB
