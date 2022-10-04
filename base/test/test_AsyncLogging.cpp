#include "../AsyncLogging.h"
#include <iostream>
#include <string>
#include <unistd.h>

int main() {
    // 后端
    TCB::AsyncLogging alog("logfile", 10 * 1024);

    std::string s1("Hello");
    alog.append(s1.c_str(), s1.size());

    alog.append("\n", 1);

    for (int i = 0; i < 4096; i++) {
        std::string s2(1024, 'a' + i % ('z' - 'a'));
        alog.append(s2.c_str(), s2.size());
        alog.append("\n", 1);
    }
    sleep(3);
    for (int i = 0; i < 4096; i++) {
        std::string s2(1024, 'a' + i % ('z' - 'a'));
        alog.append(s2.c_str(), s2.size());
        alog.append("\n", 1);
    }

    return 0;
}