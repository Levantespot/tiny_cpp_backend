#include "../LogStream.cpp"
#include <vector>
#include <iostream>

int main() {
    TCB::LogStream l;

    const char *array_string[5] = {"Hello, ", "my ", "name ", "is ", "Lee."};

    for (int i = 0; i < 5; i ++) {
        l.append(array_string[i], strlen(array_string[i]));
        // std::cout << array_string[i] << std::endl;
    }
    
    const char* s = l.get_buffer().get_data();
    size_t size = l.get_buffer().size();

    for (int i = 0; i < size+1; i++) {
        std::cout << s[i] ;
    }
    std::cout << std::endl;


    return 0;
}