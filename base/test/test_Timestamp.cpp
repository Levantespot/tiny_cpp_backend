#include "../Timestamp.h"
// #include "../LogStream.h"
#include <unistd.h> // sleep
#include <iostream>

int main() {
    TCB::Timestamp ts1;
    TCB::Timestamp ts2;
    TCB::Timestamp ts3;
    usleep(5000);
    TCB::Timestamp ts4;

    std::cout << ts1.toString() << std::endl;
    std::cout << ts2.toString() << std::endl;
    std::cout << ts3.toFormattedString(false) << std::endl;
    std::cout << ts3.toFormattedString() << std::endl;
    std::cout << ts4.toFormattedString() << std::endl;

    return 0;
}