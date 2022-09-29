#include "../LogStream.h"
#include <iostream>
#include <vector>
#include <string>

int main() {
    TCB::LogStream l;

    const char *array_string[5] = {"Hello, ", "my ", "name ", "is ", "Lee."};
    for (int i = 0; i < 5; i ++) {
        l << array_string[i];
    }
    
    l << '\n' ;

    std::vector<std::string> v_s{"Hello, ", "my ", "name ", "is ", "Lee."};
    for (int i = 0; i < 5; i ++) {
        l << v_s[i];
    }

    l << '\n';

    std::vector<const char *> v_c{"Hello, ", "my ", "name ", "is ", "Lee."};
    for (int i = 0; i < 5; i ++) {
        l << v_c[i];
    }
    
    l << '\n';

    l << std::string(3000, '-');
    l << std::string(800, '-');
    l << std::string(200, '-');

    TCB::LogStream::Buffer buffer;
    char cs[] = "Hello, my name is not Lee.";
    buffer.append(cs, strlen(cs));

    l << buffer;

    const char* s = l.get_buffer().get_data();
    size_t size = l.get_buffer().size();

    for (size_t i = 0; i < size+1; i++) {
        std::cout << s[i] ;
    }
    std::cout << std::endl;


    return 0;
}