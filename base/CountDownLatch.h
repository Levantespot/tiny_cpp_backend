#ifndef TCB_BASE_COUNTDOWNLATCH_H
#define TCB_BASE_COUNTDOWNLATCH_H

#include <mutex>
#include <condition_variable>
#include "noncopyable.h"

namespace TCB
{

// 控制线程的先后顺序。
// 例如，线程 0 需要等待其他 N 个线程（1~N）结束后才能运行。
// 实例化一个 CountDownLatch latch(N);
// 线程 0 调用 latch.wait(); 其他线程完成后调用 latch.countDown();
class CountDownLatch : noncopyable
{
public:
    explicit CountDownLatch(int count) : count_(count) {}
    void wait() // 等待其他线程
    {
        std::unique_lock<std::mutex> ulk(mutex_);
        cond_.wait(ulk, [this] { return count_ == 0; });
    }
    void countDown() // 其他线程完成任务后调用
    {
        std::lock_guard<std::mutex> lk(mutex_);
        count_--;
        if (count_ == 0) {
            cond_.notify_all();
        }
    }
    int64_t getCount() const // 查看当前还有多少线程没有完成任务
    {
        std::lock_guard<std::mutex> lk(mutex_);
        return count_;
    }
private:
    mutable std::mutex mutex_;
    std::condition_variable cond_;
    int count_;
}; // class CountDownLatch


} // namespace TCB

#endif // #ifndef TCB_BASE_COUNTDOWNLATCH_H