add_executable(hello
    1_hello.cpp
)

target_link_libraries(hello
    seg
)

add_executable(multi_thread
    2_multi_thread.cpp
)

target_link_libraries(multi_thread
    seg
    pthread
)