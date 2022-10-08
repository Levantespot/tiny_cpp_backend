#include "../LogFile.h"
#include "../Logger.h"
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <thread>
#include "../Timer.h"
#include <vector>

long g_total;
FILE* g_file;
std::unique_ptr<TCB::LogFile> g_logFile;

void dummyOutput(const char* msg, int len)
{
    g_total += len;
    if (g_file)
    {
        fwrite(msg, 1, len, g_file);
    }
    else if (g_logFile)
    {
        g_logFile->append(msg, len);
    }
}

void bench(const char* type)
{
    TCB::Logger::setOutput(dummyOutput);
    TCB::Timestamp start(TCB::Timestamp::now());
    g_total = 0;

    int n = 5*1000*1000;
    const bool kLongLog = false;
    std::string empty = " ";
    std::string longStr(3000, 'X');
    longStr += " ";
    for (int i = 0; i < n; ++i)
    {
        LOG_INFO << "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz"
                << (kLongLog ? longStr : empty)
                << i;
    }
    TCB::Timestamp end(TCB::Timestamp::now());
    double seconds = (end - start) / 1000000.0;
    printf("%12s: %f seconds, %ld bytes, %10.2f msg/s, %.2f MiB/s\n",
            type, seconds, g_total, n / seconds, g_total / seconds / (1024 * 1024));
}

// void bench_ratio(int N = 1000 * 1000) {
//     TCB::Timer timer;
//     std::vector<int> elapses;

//     timer.tik();
//     for (int i = 0; i < N; i++) {
//         TCB::Timestamp tmp(TCB::Timestamp::now());
//     }
//     timer.tok();
//     elapses.push_back(timer.elapse());

//     timer.tik();
//     for (int i = 0; i < N; i++) {
//         TCB::LogStream tmp;
//         // tmp << " ";
//     }
//     timer.tok();
//     elapses.push_back(timer.elapse());

//     timer.tik();
//     for (int i = 0; i < N; i++) {
//         TCB::Logger::SourceFile tmp(__FILE__);
//     }
//     timer.tok();
//     elapses.push_back(timer.elapse());

//     TCB::Timestamp time_(TCB::Timestamp::now());
//     timer.tik();
//     for (int i = 0; i < N; i++) {
//         time_.toFormattedString();
//     }
//     timer.tok();
//     elapses.push_back(timer.elapse());

//     std::vector<TCB::LogStream> vL(N);
//     timer.tik();
//     for (int i = 0; i < N; i++) {
//         vL[i] << "Hello" << "abcdefghijklmnopqrstuvwxwz"
//             << "1234567890" ;
//     }
//     timer.tok();
//     elapses.push_back(timer.elapse());

//     int sum = 0;
//     for (auto it : elapses) {
//         sum += it;
//     }
//     std::cout << "Timestamp\tLogStream\tSourceFile\tFormat\t\t<<" << '\n';
//     for (auto it : elapses) {
//         std::cout << it * 1.0 / sum << '\t';
//     }
//     std::cout << '\n';
// }

int main() {
    // 测试各种等级
    LOG_TRACE << "trace";
    LOG_DEBUG << "debug";
    LOG_INFO << "Hello";
    LOG_WARN << "World";
    LOG_ERROR << "Error";
    LOG_INFO << sizeof(TCB::Logger);
    LOG_INFO << sizeof(TCB::LogStream);
    // LOG_INFO << sizeof(TCB::LogStream::Buffer);

    // 测试线程写日志
    std::thread t([] () {
        LOG_INFO << "logInThread";
        usleep(1000);
    });
    t.join();

    sleep(1);
    // 不写入硬盘
    bench("nop");

    char buffer[64*1024];

    sleep(1);
    // 写入 /dev/null
    g_file = fopen("/dev/null", "w");
    setbuffer(g_file, buffer, sizeof buffer);
    bench("/dev/null");
    fclose(g_file);

    sleep(1);
    // 写入文件（不保证线程安全）
    g_file = NULL;
    g_logFile.reset(new TCB::LogFile("test_log_st", 500*1000*1000, false));
    bench("test_log_st");

    sleep(1);
    // 写入文件（保证线程安全）
    g_logFile.reset(new TCB::LogFile("test_log_mt", 500*1000*1000, true));
    bench("test_log_mt");
    g_logFile.reset();

    // // 测试所占比例
    // bench_ratio();
}