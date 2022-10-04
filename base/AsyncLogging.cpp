#include "AsyncLogging.h"

namespace TCB
{

AsyncLogging::AsyncLogging(const std::string& basename,
                            size_t rollSize,
                            int flushInterval)
  : basename_(basename),
    rollSize_(rollSize),
    flushInterval_(flushInterval),
    running_(true),
    mutex_(),
    latch_(1),
    lock_(mutex_, std::defer_lock),
    cond_(),
    currentBuffer_(new Buffer),
    nextBuffer_(new Buffer),
    buffers_(),
    thread_(std::bind(&AsyncLogging::threadFunc, this))
{   
    buffers_.reserve(16);
    output.reset(new LogFile(basename_, rollSize_, false));
    latch_.countDown();
}

AsyncLogging::~AsyncLogging()
{   
    std::cout << "a\n" ; // DEBUG
    if (running_) {
        running_ = false;
        std::cout << "b\n" ; // DEBUG
        cond_.notify_one(); // 通知线程写入剩余部分
        if (thread_.joinable()) {
            std::cout << "c\n" ; // DEBUG
            thread_.join();
        }
    }
    std::cout << "d\n" ; // DEBUG
}

void AsyncLogging::append(const char* logline, size_t len) {
    if (!running_) {
        std::cerr << "Not running!" << '\n';
        return;
    }

    std::lock_guard<std::mutex> lk(mutex_);

    if (currentBuffer_->avail() > len) { // 当前的缓存可以容纳
        currentBuffer_->append(logline, len);
    } else { // 无法容纳，将当前缓存挂在完成队列中，切换使用备用缓存
        buffers_.push_back(std::move(currentBuffer_));

        if (nextBuffer_) {
            currentBuffer_ = std::move(nextBuffer_);
        } else {
            currentBuffer_.reset(new Buffer);
        }
        currentBuffer_->append(logline, len);
        cond_.notify_one(); // 通知线程有数据可写
    }
}

void AsyncLogging::flush() {
    output->flush();
}

void AsyncLogging::threadFunc() {
    latch_.wait(); // 等待父线程初始化资源

    BufferPtr newBuffer(new Buffer);
    BufferVector buffersToWrite;
    buffersToWrite.reserve(16);
    std::cout << "1\n" ; // DEBUG
    while (running_) {
        std::cout << "2\n" ; // DEBUG
        lock_.lock();
        cond_.wait(lock_, [this] () {
            return !running_ || !buffers_.empty(); 
        });
        std::cout << "3\n" ; // DEBUG

        if (!running_) { // 退出循环
            std::cout << "3.5\n" ; // DEBUG
            lock_.unlock();
            std::cout << "3.55" << std::endl; // DEBUG
            break;
        }
        std::cout << "4\n" ; // DEBUG

        // 交换 buffers (condition race)
        buffersToWrite.swap(buffers_);
        if (!nextBuffer_) { // 补充
            nextBuffer_ = std::move(newBuffer);
        }
        lock_.unlock(); // 结束互斥区域

        // 写入目的地
        for (const auto& bf : buffersToWrite) {
            output->append(bf->get_data(), bf->size());
        }

        // 补充 buffer
        if (!newBuffer) {
            newBuffer = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            newBuffer->reset();
        }

        buffersToWrite.clear();
        flush();
    }
    std::cout << "5\n" ; // DEBUG
    // 写入剩余部分 (condition race)
    {
        std::lock_guard<std::mutex> lk(mutex_);
        if (currentBuffer_ && currentBuffer_->size()) {
            buffersToWrite.push_back(std::move(currentBuffer_));
        }
        if (nextBuffer_ && nextBuffer_->size()) {
            buffersToWrite.push_back(std::move(nextBuffer_));
        }
    }
    for (const auto& bf : buffersToWrite) {
        output->append(bf->get_data(), bf->size());
    }
    flush();
    std::cout << "6\n" ; // DEBUG
}

} // namespace TCB
