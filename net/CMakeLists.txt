aux_source_directory(. TCB_NET_BASE_FILES)

set(TCB_NET_FILES
    ${TCB_NET_BASE_FILES}
    poller/DefaultPoller.cpp
    poller/PollPoller.cpp
)

add_library(tcb_net STATIC ${TCB_NET_FILES})
target_link_libraries(tcb_net tcb_base pthread)

add_subdirectory(test)