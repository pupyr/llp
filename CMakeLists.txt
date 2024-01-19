cmake_minimum_required(VERSION 3.26)
project(llp)

set(CMAKE_CXX_STANDARD 17)
link_libraries(ws2_32 wsock32)

add_executable(llp
        reader.c
        fileStruct.c
        alloc.c
        parser.c
        server.h
        server.c
)

add_executable(llp2
        client.c
        transport.h
)

add_executable(llp_test_reg
    test.c
)