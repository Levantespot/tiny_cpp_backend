add_executable(test_EventLoop test_EventLoop.cpp)
target_link_libraries(test_EventLoop tcb_net)

add_executable(test_Channel test_Channel.cpp)
target_link_libraries(test_Channel tcb_net)

add_executable(test_timerfd test_timerfd.cpp)
target_link_libraries(test_timerfd tcb_net)