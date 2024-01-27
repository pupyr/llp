cmake_minimum_required(VERSION 3.26)
project(llp)

set(CMAKE_CXX_STANDARD 17)
link_libraries(ws2_32 wsock32)

add_executable(llp_xml
        reader.h
        fileStruct.h
        alloc.h
        parser.h
        reader.c
        fileStruct.c
        alloc.c
        parser.c
        server.h
        server.c
        transport.h
)

add_executable(llp_xml_client
        client.c
        xml.c
        transport.h
        xml.h
)