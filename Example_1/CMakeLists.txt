cmake_minimum_required(VERSION 3.1)

project(Example_1)

set(CMAKE_CXX_STANDARD 14)

## Source files
set(SRCS
    main.cpp
    )

add_executable(${PROJECT_NAME} ${SRCS})


target_link_libraries(${PROJECT_NAME}
    #    MyZipHelper
    ${ZLIB_INCLUDE_DIR}
    )
