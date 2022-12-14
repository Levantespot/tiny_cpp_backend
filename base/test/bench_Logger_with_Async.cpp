#include <stdio.h>
#include <unistd.h>

#include <string>

#include "base/AsyncLogging.h"
#include "base/Logger.h"

long g_total;
std::unique_ptr<TCB::AsyncLogging> plog;

void dummyOutput(const char* msg, int len) {
  g_total += len;
  plog->append(msg, len);
}

void bench(const char* type) {
  TCB::Logger::setOutput(dummyOutput);
  // TCB::Logger::setFlush(std::bind(&TCB::AsyncLogging::flush, plog.get())); // 不用设置 Flush
  TCB::Timestamp start;
  g_total = 0;

  int n = 5 * 1000 * 1000;
  const bool kLongLog = false;
  std::string empty = " ";
  std::string longStr(3000, 'X');
  longStr += " ";
  for (int i = 0; i < n; ++i) {
    LOG_INFO << "Hello 0123456789"
             << " abcdefghijklmnopqrstuvwxyz" << (kLongLog ? longStr : empty) << i;
  }
  TCB::Timestamp end;
  double seconds = (end - start) / 1000000.0;
  printf("%12s: %f seconds, %ld bytes, %10.2f msg/s, %.2f MiB/s\n", type, seconds, g_total,
         n / seconds, g_total / seconds / (1024 * 1024));
}

int main() {
  plog.reset(new TCB::AsyncLogging("logfile", 2 * TCB::kLargeBuffer));
  bench("AsyncLogging");
}