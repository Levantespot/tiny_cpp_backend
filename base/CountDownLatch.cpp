#include "CountDownLatch.h"

namespace TCB
{

CountDownLatch::CountDownLatch(int count) : count_(count) {}

void CountDownLatch::wait() {
    std::unique_lock<std::mutex> ulk(mutex_);
    cond_.wait(ulk, [this] { return count_ == 0; });
}

void CountDownLatch::countDown() {
    std::lock_guard<std::mutex> lk(mutex_);
    count_--;
    if (count_ == 0) {
        cond_.notify_all();
    }
}

int64_t CountDownLatch::getCount() const {
    std::lock_guard<std::mutex> lk(mutex_);
    return count_;
}


} // namespace TCB
