aux_source_directory(. TCB_BASE_FILES)

add_library(tcb_base STATIC ${TCB_BASE_FILES})
target_link_libraries(tcb_base pthread)

add_subdirectory(test)

target_include_directories(tcb_base PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_BINARY_DIR}/../"
                           )