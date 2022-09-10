#ifndef TCB_BASE_COUNTDOWNLATCH_H
#define TCB_BASE_COUNTDOWNLATCH_H

#include <mutex>
#include <condition_variable>
#include "noncopyable.h"

namespace TCB
{

// 允许一个或者多个线程去等待其他线程完成操作。
class CountDownLatch : noncopyable
{
public:
    explicit CountDownLatch(int count);
    void wait(); // 等待其他线程
    void countDown(); // 其他线程完成任务后调用
    int64_t getCount() const; // 查看当前还有多少线程没有完成任务

private:
    mutable std::mutex mutex_;
    std::condition_variable cond_;
    int count_;
}; // class CountDownLatch


} // namespace TCB

#endif // #ifndef TCB_BASE_COUNTDOWNLATCH_H