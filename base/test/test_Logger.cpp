#include "../Logger.h"
#include <iostream>

int main() {
    LOG_INFO << "Hello, world";

    LOG_ERROR << "My name is Lee";

    LOG_FATAL << "This will raise an abort" ;

    return 0;
}