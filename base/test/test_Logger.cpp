#include "../Logger.cpp"
#include "../LogStream.cpp"
#include "../Timestamp.cpp"
#include <iostream>

int main() {
    LOG_INFO << "Hello, world";

    LOG_ERROR << "My name is Lee";

    return 0;
}