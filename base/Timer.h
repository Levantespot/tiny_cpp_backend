#include <chrono>

namespace TCB
{

// 一个简单的计时器
class Timer
{
public:
    void tik() {
        tik_t = std::chrono::steady_clock::now();
    }
    void tok() {
        tok_t = std::chrono::steady_clock::now();
    }
    int elapse() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(tok_t - tik_t).count();;
    }
private:
    std::chrono::_V2::steady_clock::time_point tik_t;
    std::chrono::_V2::steady_clock::time_point tok_t;

}; // class Timer

} // namespace TCB

