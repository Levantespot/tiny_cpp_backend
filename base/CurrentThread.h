#ifndef TCB_CURRENTTHREAD_H
#define TCB_CURRENTTHREAD_H

#include <unistd.h>

#include <thread>

namespace TCB {
namespace CurrentThread {
extern thread_local int t_cachedTid;
extern thread_local char t_tidString[32];
extern thread_local std::size_t t_tidLength;

void inline cacheTid() {
  t_cachedTid = gettid();
  t_tidLength = snprintf(t_tidString, 32, "%d", t_cachedTid);
}

inline int tid() {
  if (t_cachedTid == 0) {
    cacheTid();
  }
  return t_cachedTid;
}

inline const char* tidString() {
  if (t_cachedTid == 0) {
    cacheTid();
  }
  return t_tidString;
}

inline std::size_t tidLength() {
  if (t_cachedTid == 0) {
    cacheTid();
  }
  return t_tidLength;
}

}  // namespace CurrentThread
}  // namespace TCB

#endif