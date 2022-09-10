#ifndef TCB_BASE_ASYNCLOGGING_H
#define TCB_BASE_ASYNCLOGGING_H

#include "noncopyable.h"
#include <mutex>
#include <condition_variable>
#include <CountDownLatch.h>
#include <vector>
#include <string>

namespace TCB
{

class AsyncLogging : noncopyable {

public:
    AsyncLogging();
    ~AsyncLogging();
    void append();
    void stop();

private:
    const int buffer_size = 4096;
    using Buffer = std::string;
    using BufferVector = std::vector<std::unique_ptr<Buffer>>;

}; // class AsyncLogging

} // namespace TCB


#endif // #ifndef TCB_BASE_ASYNCLOGGING_H