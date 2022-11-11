#include <string>
#include <thread>

#include "base/FileUtil.h"

int main() {
  TCB::FileUtil::AppendFile af("logfile.log");

  /* one thread */
  // const char* s1 = "Hello, \n";
  // const char* s2 = "world!\n";

  // af.append(s1, strlen(s1));
  // af.append(s2, strlen(s2));

  /* multi thread */
  // int size = 1000 * 1000;
  int size = 64;
  std::thread t1([&] {
    std::string s(size, 'a');
    s += '\n';
    af.append(s.c_str(), size + 1);
  });
  std::thread t2([&] {
    std::string s(size, 'b');
    s += '\n';
    af.append(s.c_str(), size + 1);
  });
  std::thread t3([&] {
    std::string s(size, 'c');
    s += '\n';
    af.append(s.c_str(), size + 1);
  });
  std::thread t4([&] {
    std::string s(size, 'd');
    s += '\n';
    af.append(s.c_str(), size + 1);
  });
  std::thread t5([&] {
    std::string s(size, 'e');
    s += '\n';
    af.append(s.c_str(), size + 1);
  });

  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();

  return 0;
}