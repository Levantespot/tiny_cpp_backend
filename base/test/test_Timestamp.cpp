#include "../Timestamp.cpp"
#include "../LogStream.h"
#include <unistd.h> // sleep
#include <iostream>

int main() {
    TCB::Timestamp ts1;
    TCB::Timestamp ts2(10005000);
    TCB::Timestamp ts3(TCB::Timestamp::now());
    usleep(5000);
    TCB::Timestamp ts4(TCB::Timestamp::now());

    std::cout << ts1.toString() << std::endl;
    std::cout << ts2.toString() << std::endl;
    std::cout << ts3.toFormattedString(false) << std::endl;
    std::cout << ts3.toFormattedString() << std::endl;
    std::cout << ts4.toFormattedString() << std::endl;

    return 0;
}