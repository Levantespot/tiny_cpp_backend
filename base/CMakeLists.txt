include_directories(${PROJECT_SOURCE_DIR})

aux_source_directory(. TCB_BASE_FILES)

add_library(tcb_base SHARED ${TCB_BASE_FILES})
target_link_libraries(tcb_base pthread)

add_subdirectory(test)